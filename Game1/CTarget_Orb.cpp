#include "stdafx.h"
#include "CTarget_Orb.h"
#include "CBossPawn.h"
#include "CBoss_Controller.h"

CTarget_Orb::CTarget_Orb(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
    m_Pos = _Info.SpawnPos;
    m_StartPos = m_Pos;

    m_Skill_Info.Instigator = _Info.Instigator;
    m_Direction = _Info.Aim_Dir;
    if (m_Direction.LengthSquared() < 1e-6f)
        m_Direction = Vec2(1.f, 0.f);

    // 충돌체
    {
        m_Collision = std::make_shared<ObCircle>();
        m_Collision->scale = Vec2(60.f, 60.f);
        m_Collision->isFilled = false;
        m_Collision->visible = false;
        m_Collision->collider = COLLIDER::CIRCLE;
        m_Collision->Set_Owner(this);
        m_Collision->Set_Instigator(m_Skill_Info.Instigator); 
        m_Collision->SetParentRT(*m_Skill_Info.Instigator->Get_Collision());

        COLLISION->AddCollider(m_Collision.get());

        m_Collision->SetLocalPos(Vec2(cosf(_Info.Angle) * 100.f, sinf(_Info.Angle) * 100.f));
    }

    // 이미지
    {
        m_OrbImage_Small = RESOURCE->CreateImage(L"Monster\\Lightning\\LightningBossTargetIdle.png",
            Vec2(20.f, 11.f) * 3.f, true);
        m_OrbImage_Small->SetParentRT(*m_Collision);
        m_OrbImage_Small->SetLocalPosY(20.f);

        m_OrbImage_Big = RESOURCE->CreateSprite(L"Monster\\Lightning\\Target_Orb_Full.png", Vec2(64.f / 2.f, 32.f) * 3.f, Int2(2, 1), false);
        m_OrbImage_Big->SetParentRT(*m_Collision);
        m_OrbImage_Big->SetLocalPosY(20.f);

        Vec2 size = Vec2(32.f, 28.f) * 3.f;
        m_OrbImage_Hit = RESOURCE->CreateImage(L"Monster\\Lightning\\LightningBossTargetCharged.png", size);
        m_OrbImage_Hit->SetParentRT(*m_Collision);
    }
    
    m_OrbShadow = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(75.f), false);
    m_OrbShadow->SetParentRT(*m_Collision);
    m_OrbShadow->color = Color(0.5f, 0.5f, 0.5f, 0.25f);
    m_OrbShadow->visible = true;
   

    m_IsThrow = false; // 생성 직후 일단 대기

    m_CurImg = m_OrbImage_Small;
}

CTarget_Orb::~CTarget_Orb()
{
    Release();
}

void CTarget_Orb::BeginPlay()
{
    CSkill_Actor::BeginPlay();

    m_Controller = dynamic_cast<CBossPawn*>(m_Skill_Info.Instigator)->GetController();

    m_Dest = m_Collision ? m_Collision->GetWorldPos() : m_Pos;
}

void CTarget_Orb::Tick()
{
    CSkill_Actor::Tick();
    if (m_Destroy) return;

    m_Life -= DELTA;
    if (m_Life <= 0.f)
        m_Destroy = true;

    if (m_prevDestroy)
    {
        m_boomTimer -= DELTA;

        if (m_boomTimer <= 0.f)
        {
            m_Destroy = true;
        }
    }

    if (m_IsThrow && m_Collision)
    {
        Vec2 colPos = m_Collision->GetWorldPos();
        Vec2 toTarget = m_Dest - colPos;
        float distSq = toTarget.LengthSquared();

        if (distSq <= m_StopRadius * m_StopRadius)
        {
            m_IsThrow = false;
            m_CanHit = true;

            m_OrbImage_Small->visible = false;
            m_OrbImage_Big->visible = true;
            m_OrbImage_Big->frame = Int2(0, 0);
            m_OrbImage_Big->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

            m_CurImg = m_OrbImage_Big;

            m_Controller->OnJavelinStart.Broadcast(m_Collision->GetWorldPos());

            m_Collision->scale = Vec2(160.f, 130.f);
        	m_OrbShadow->scale = Vec2(180.f, 150.f);
        }
        else
        {
            if (toTarget.LengthSquared() > 1e-6f)
                toTarget.Normalize();

            float speedNow = m_Speed;
            float dist = sqrtf(distSq);
            if (dist < m_SlowRadius)
            {
                float t = dist / m_SlowRadius;               
                speedNow = max(m_MinSpeed, m_Speed * t);     
            }

            m_Direction = (m_Direction * 0.8f + toTarget * 0.2f);
            if (m_Direction.LengthSquared() > 1e-6f)
                m_Direction.Normalize();

            m_Collision->MoveWorldPos(m_Direction * speedNow * DELTA);
        }
    }

	if (m_Collision)
		m_Collision->Update();

    m_OrbImage_Big->Update();
    m_CurImg->Update();
    m_OrbImage_Hit->Update();

	if (m_OrbShadow)
		m_OrbShadow->Update();

}


void CTarget_Orb::LateUpdate()
{
    CSkill_Actor::LateUpdate();
}

void CTarget_Orb::Render()
{
    CSkill_Actor::Render();

    if (m_Destroy) return;

    m_OrbShadow->Render();
    m_Collision->Render();

    m_CurImg->Render();
    m_OrbImage_Big->Render();
    m_OrbImage_Hit->Render();

}

void CTarget_Orb::Release()
{
    CSkill_Actor::Release();

    if (m_Collision)
        COLLISION->RemoveCollider(m_Collision.get());

    m_Collision.reset();
    m_OrbImage_Small.reset();
    m_OrbShadow.reset();
}

bool CTarget_Orb::CheckHitWall()
{
    return CSkill_Actor::CheckHitWall();
}

void CTarget_Orb::OnHitWall(const Vec2& hitPos)
{
    CSkill_Actor::OnHitWall(hitPos);

	//m_Destroy = true; 
}

void CTarget_Orb::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
    CSkill_Actor::OnComponentBeginOverlap(collider, other);

    if (m_Destroy) return;

    CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
    if (!hit) return;


    if (dynamic_cast<CBossPawn*>(hit)) return;

    if (hit == m_Skill_Info.Instigator) return;

    if (hit == m_Skill_Info.Target)
    {
        if (hit->IsKnockbackPrev())
        {
            Vec2 dir = hit->Get_WorldPos() - m_StartPos;
            if (dir.LengthSquared() > 0.0001f) dir.Normalize();
            hit->Get_Collision()->MoveWorldPos(dir * m_Skill_Info.Amount * DELTA);
        }

        FHitDamage hitDamage;
        hitDamage.Damage = m_Skill_Info.Damage;
        hitDamage.colldier = collider;
        hitDamage.other = other;

        hit->TakeDamage(hitDamage);

        // 타격 시 제거
        //m_Destroy = true;
    }
}

void CTarget_Orb::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
    CSkill_Actor::OnComponentEndOverlap(collider, other);
}

void CTarget_Orb::ThrowTowards(const Vec2& worldTarget)
{
    m_Dest = worldTarget;

    Vec2 myPos = m_Collision ? m_Collision->GetWorldPos() : m_Pos;
    Vec2 dir = m_Dest - myPos;
    if (dir.LengthSquared() > 0.0001f)
        dir.Normalize();

    m_Direction = dir;
    m_IsThrow = true;
}

void CTarget_Orb::OnHitSpear()
{
	if (m_CanHit == true)
	{
        m_prevDestroy = true;
        m_boomTimer = 0.3f;

        SOUND->StopPlay("ImpactLightning", 0.15f);

        PARTICLE->Spawn_ParticleDir(L"ShockSpark_AS", m_Collision->GetWorldPos(), Vec2(1.2f, 1.5f));
        PARTICLE->Spawn_ParticleDir(L"Light_Long", Vec2(m_Collision->GetWorldPos().x, m_Collision->GetWorldPos().y + 340.f));

        m_OrbImage_Big->visible = false;
        m_OrbImage_Hit->visible = true;

        m_CurImg = m_OrbImage_Hit;
	}
}
