#include "stdafx.h"
#include "CBat_Ball.h"

#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CBoss_TwinGirl.h"
#include "CBoss_TwinMan.h"
#include "CPlayer.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"
#include "CResourceManager.h"

CBat_Ball::CBat_Ball(FSkillInfo& _Info)
    : CSkill_Actor(_Info)
{
    m_Pos = _Info.SpawnPos;
    m_StartPos = m_Pos;

    // 충돌체
    {
        m_Collision = make_shared<ObCircle>();
        m_Collision->scale = Vec2(40.f, 40.f);
        m_Collision->SetWorldPos(Vec2(m_Pos.x - 50.f, m_Pos.y));
        m_Collision->visible = false;
        m_Collision->collider = COLLIDER::CIRCLE;
        m_Collision->isFilled = false;
        m_Collision->Set_Owner(this);
        m_Collision->Set_Instigator(m_Skill_Info.Instigator);
        COLLISION->AddCollider(m_Collision.get());
    }

    m_Batball = RESOURCE->CreateSprite(
        L"Monster\\Lightning\\MetalOrb.png",
        Vec2(128.f / 4.f, 32.f) * 3.f,
        Int2(4, 1),
        true
    );
    m_Batball->SetParentRT(*m_Collision);
    m_Batball->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
}

CBat_Ball::~CBat_Ball()
{
    Release();
}

void CBat_Ball::BeginPlay()
{
    CSkill_Actor::BeginPlay();

    m_Start = m_Pos;
    m_Target = m_Skill_Info.Target ? m_Skill_Info.Target->Get_WorldPos() : m_Pos;

    const float T = max(m_FlightTime, 1e-3f); 
    Vec2 r = m_Target - m_Start;
    Vec2 g = Vec2(0.f, m_GravityY);
    m_V0 = (r - 0.5f * g * T * T) / T;

    m_Arrived = false;
    m_Timer = 0.f;
    m_LifeTime = 0.f;
    m_AfterArrive = 0.f;

    PARTICLE->Spawn_ParticleDir(L"ShockSpark_AS", m_Pos, Vec2(1.3f, 1.5f));
}

void CBat_Ball::Tick()
{
    CSkill_Actor::Tick();

    if (m_PendingKill)
    {
        KillNow();
        return;
    }

    m_LifeTime += DELTA;
    if (m_LifeTime >= kMaxLife)
    {
        m_PendingKill = true; 
        return;
    }

    if (m_Arrived)
    {
        m_AfterArrive += DELTA;
        if (m_AfterArrive >= kDespawnGrace)
        {
            m_PendingKill = true; 
            return;
        }

        if (m_Batball)  m_Batball->Update();
        if (m_Collision) m_Collision->Update();
        return;
    }

    m_Timer += DELTA;
    const float t = min(m_Timer, m_FlightTime);

    Vec2 g = Vec2(0.f, m_GravityY);
    Vec2 pos = m_Start + m_V0 * t + 0.5f * g * t * t;

    if (m_Collision)
    {
        m_Collision->SetWorldPos(pos);
        m_Collision->Update();
    }

    if (m_Timer >= m_FlightTime)
    {
        if (m_Collision) m_Collision->SetWorldPos(m_Target);

        m_Arrived = true;
        m_AfterArrive = 0.f;

        if (auto owner = dynamic_cast<CBossPawn*>(m_Skill_Info.Instigator))
        {
            owner->GetController()->OnDashStart.Broadcast(m_Collision ? m_Collision->GetWorldPos() : m_Target);
        }
    }

    if (m_Batball) m_Batball->Update();
}

void CBat_Ball::LateUpdate()
{
    CSkill_Actor::LateUpdate();
}

void CBat_Ball::Render()
{
    CSkill_Actor::Render();

    if (m_PendingKill) return;

    if (m_Collision) m_Collision->Render();
    if (m_Batball)  m_Batball->Render();
}

void CBat_Ball::Release()
{
    CSkill_Actor::Release();

    if (m_Collision)
    {
        COLLISION->RemoveCollider(m_Collision.get());
        m_Collision.reset();
    }
    m_Batball.reset();
}

bool CBat_Ball::CheckHitWall()
{
    return CSkill_Actor::CheckHitWall();
}

void CBat_Ball::OnHitWall(const Vec2& hitPos)
{
    CSkill_Actor::OnHitWall(hitPos);

    m_PendingKill = true;
}

void CBat_Ball::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
    CSkill_Actor::OnComponentBeginOverlap(collider, other);

    if (m_PendingKill) return;

    CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
    if (!hit) return;

    if (m_Arrived)
    {
        if (auto girl = dynamic_cast<CBoss_TwinGirl*>(hit))
        {
            if (girl != m_Skill_Info.Instigator) // 인스티게이터(TwinMan)가 아닐 경우
            {
                Vec2 randomVec = Vec2((float)(rand() % 20), (float)(rand() % 40));
                Vec2 particlePos = m_Collision ? m_Collision->GetWorldPos() : m_Target;

                PARTICLE->Spawn_ParticleDir(L"HitSpark", particlePos + randomVec, Vec2(0.6f));

                m_PendingKill = true; 
                // KillNow() 호출 제거
            }
        }
        return;
    }

    // 2. 도착 전: 플레이어 타격 로직
    if (dynamic_cast<CBoss_TwinMan*>(hit)) return; // 트윈맨 무시
    if (hit == m_Skill_Info.Instigator)     return; // 자기자신 무시

    auto* player = dynamic_cast<CPlayer*>(hit);
    if (!player) return;

    // 다중 히트 방지 (이미 타격한 경우)
    if (m_HitObjects.find(other) != m_HitObjects.end())
        return;
    m_HitObjects.insert(other);

    // 넉백 로직
    if (player->IsKnockback())
    {
        Vec2 dir = player->Get_WorldPos() - m_StartPos;
        if (dir.LengthSquared() > 1e-6f) dir.Normalize();
        // MoveWorldPos(dir * m_Skill_Info.Amount * DELTA)는 Hit 콜백에서 부적절함.
        // TakeDamage 내부에서 넉백 처리를 하는 것이 일반적입니다. (임시 제거)
        // player->Get_Collision()->MoveWorldPos(dir * m_Skill_Info.Amount * DELTA); 
    }

    // 데미지 전달
    FHitDamage hitDamage;
    hitDamage.Damage = m_Skill_Info.Damage;
    hitDamage.colldier = collider;
    hitDamage.other = other;

    // 넉백 정보는 FHitDamage에 담아서 전달
    if (player->IsKnockbackPrev())
    {
        hitDamage.Amount = 700.f; // 넉백량 (FSkillInfo에서 받아오는 것이 좋음)
        hitDamage.Duration = 0.25f; // 넉백 시간
        Vec2 dir = player->Get_WorldPos() - m_StartPos;
        if (dir.LengthSquared() > 1e-6f) dir.Normalize();
        hitDamage.Dir = dir;
    }

    player->TakeDamage(hitDamage);

    // 타격 시 즉시 제거를 원한다면 플래그 설정
    m_PendingKill = true;
    // KillNow() 호출 제거
}

void CBat_Ball::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
    CSkill_Actor::OnComponentEndOverlap(collider, other);
}

void CBat_Ball::KillNow()
{
    // 콜라이더와 비주얼을 제거/숨김
    if (m_Collision)
    {
        COLLISION->RemoveCollider(m_Collision.get());
        m_Collision.reset();
    }
    if (m_Batball)  m_Batball->visible = false;

    // 추가 파괴 이펙트/사운드 삽입 가능
}