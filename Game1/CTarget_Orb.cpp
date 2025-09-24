#include "stdafx.h"
#include "CTarget_Orb.h"

#include "CBossPawn.h"
#include "CBoss_Controller.h"

CTarget_Orb::CTarget_Orb(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	m_Skill_Info.Instigator = _Info.Instigator;

	m_Target_Orb.resize(TARGET_ORB);
	m_Target_Img.resize(TARGET_ORB);
	m_OrbStates.resize(TARGET_ORB);
	m_OrbShadows.resize(TARGET_ORB);

	for (int i = 0; i < TARGET_ORB; ++i)
	{
		m_Angle[i] = i * ((2 * PI) / 5) - (PI / 2);
	}

}

CTarget_Orb::~CTarget_Orb()
{
	Release();
}

void CTarget_Orb::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	m_BossController = dynamic_cast<CBossPawn*>(m_Skill_Info.Instigator)->GetController();
	if (!m_BossController) return;

	m_BossController->OnThrowTargetOrb.Add([this](int value)
		{
			this->OnThrowOrb(value);
		});


	for (int i = 0; i < TARGET_ORB; ++i)
	{
		m_Target_Orb[i] = make_shared<ObCircle>();
		m_Target_Orb[i]->scale = Vec2(60.f, 60.f);
		m_Target_Orb[i]->SetParentRT(*m_Skill_Info.Instigator->Get_Collision());
		m_Target_Orb[i]->isFilled = false;
		m_Target_Orb[i]->visible = false;
		m_Target_Orb[i]->collider = COLLIDER::CIRCLE;
		m_Target_Orb[i]->Set_Owner(this);
		m_Target_Orb[i]->SetLocalPos(Vec2(cosf(m_Angle[i]) * m_Radius, sinf(m_Angle[i]) * m_Radius));

		m_Target_Img[i] = RESOURCE->CreateImage(L"Monster\\Lightning\\LightningBossTargetIdle.png", m_OrbSize, true);
		m_Target_Img[i]->SetParentRT(*m_Target_Orb[i]);
		m_Target_Img[i]->SetLocalPosY(20.f);

		COLLISION->AddCollider(m_Target_Orb[i].get());

		m_OrbShadows[i] = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(75.f), true);
		m_OrbShadows[i]->SetParentRT(*m_Target_Orb[i]);
		m_OrbShadows[i]->color = Color(0.5f, 0.5f, 0.5f, 0.25f);
	}
}

void CTarget_Orb::Tick()
{
	CSkill_Actor::Tick();
	if (m_PendingKill) return;

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	for (int i = 0; i < static_cast<int>(m_Target_Orb.size()); ++i)
	{
		if (m_OrbStates[i].dead || !m_Target_Orb[i])
			continue;

		if (m_OrbStates[i].isHit)
		{
			m_OrbStates[i].boomTimer -= DELTA;

			if (m_OrbStates[i].boomTimer <= 0.f && !m_OrbStates[i].exploded)
			{
				Vec2 p = m_Target_Orb[i]->GetWorldPos();
				PARTICLE->Spawn_Particle(L"Light_Long", Vec2(p.x, p.y + 310.f));
				m_OrbStates[i].exploded = true;

				DestroyOrb(i);
				continue; 
			}

			if (m_Target_Orb[i])  
				m_Target_Orb[i]->Update();

			if (m_Target_Img[i]) 
				m_Target_Img[i]->Update();

			if (m_OrbShadows[i]) 
				m_OrbShadows[i]->Update();
			continue;
		}

		
		if (m_OrbStates[i].isThrow && !m_OrbStates[i].isArrived)
		{
			Vec2 cur = m_Target_Orb[i]->GetWorldPos();
			Vec2 target = m_OrbStates[i].targetPos;
			Vec2 to = target - cur;
			float distSq = to.Dot(to);
			const float STOP_DISTANCE_SQ = 25.f; 

			if (distSq <= STOP_DISTANCE_SQ)
			{
				m_OrbStates[i].isArrived = true;
				m_Target_Orb[i]->SetWorldPos(target);

				auto arrived = RESOURCE->CreateSprite(
					L"Monster\\Lightning\\Target_Orb_Full.png",
					Vec2(64.f / 2.f, 32.f) * 3.f, Int2(2, 1), true);
				arrived->SetParentRT(*m_Target_Orb[i]);
				arrived->SetLocalPosY(0.f);
				m_Target_Img[i] = arrived;
				m_Target_Img[i]->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

				m_Target_Orb[i]->scale = Vec2(90.f, 90.f);

				if (m_OrbShadows[i])
					m_OrbShadows[i]->scale = Vec2(160.f, 150.f);

				if (m_BossController)
					m_BossController->OnJavelinStart.Broadcast(m_Target_Orb[i]->GetWorldPos());
			}
			else
			{
				m_Target_Orb[i]->MoveWorldPos(m_OrbStates[i].direction * m_OrbStates[i].speed * DELTA);
			}
		}
		if (m_Target_Orb[i])
			m_Target_Orb[i]->Update();

		if (m_Target_Img[i]) 
			m_Target_Img[i]->Update();

		if (m_OrbShadows[i]) 
			m_OrbShadows[i]->Update();
	}
}


void CTarget_Orb::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CTarget_Orb::Render()
{
	CSkill_Actor::Render();

	if (m_PendingKill) return;

	for (int i = 0; i < (int)m_Target_Orb.size(); ++i)
	{
		if (m_OrbStates[i].dead) continue;
		if (m_OrbShadows[i]) m_OrbShadows[i]->Render();
		if (m_Target_Orb[i]) m_Target_Orb[i]->Render();  
		if (m_Target_Img[i]) m_Target_Img[i]->Render();
	}

}

void CTarget_Orb::Release()
{
	CSkill_Actor::Release();

	for (int i = 0; i < TARGET_ORB; ++i)
	{
		if (m_Target_Orb[i]) {
			COLLISION->RemoveCollider(m_Target_Orb[i].get());
		}
	}

	m_Target_Orb.clear();
	m_Target_Img.clear();
	m_OrbStates.clear();
	m_OrbShadows.clear();
}

bool CTarget_Orb::CheckHitWall()
{
	return CSkill_Actor::CheckHitWall();
}

void CTarget_Orb::OnHitWall(const Vec2& hitPos)
{
	CSkill_Actor::OnHitWall(hitPos);
}

void CTarget_Orb::OnThrowOrb(int value)
{
	if (value < 0 || value >= (int)m_Target_Orb.size()) return;

	if (m_OrbStates[value].dead || !m_Target_Orb[value] || !m_Target_Img[value] || !m_OrbShadows[value])
		RecreateOrb(value);

	m_Target_Orb[value]->visible = true;
	m_Target_Img[value]->visible = true;
	m_OrbShadows[value]->visible = true;

	CCharacter* target = dynamic_cast<CCharacter*>(m_Skill_Info.Target);
	if (!target) return;

	const Vec2 playerPos = target->Get_WorldPos();
	m_OrbStates[value].targetPos = playerPos;

	const Vec2 orbPos = m_Target_Orb[value]->GetWorldPos();

	Vec2 dir = playerPos - orbPos;
	float lengthSq = dir.x * dir.x + dir.y * dir.y;

	if (lengthSq > 1e-6f)
		dir.Normalize();

	else dir = Vec2(1.f, 0.f);

	m_OrbStates[value].direction = dir;
	m_OrbStates[value].speed = 1500.f;
	m_OrbStates[value].isThrow = true;

	m_HitObjects.clear();
}

void CTarget_Orb::DestroyOrb(int idx)
{
	if (!m_Target_Orb[idx]) return;

	COLLISION->RemoveCollider(m_Target_Orb[idx].get());
	m_Target_Orb[idx]->collider = COLLIDER::NONE;

	if (m_Target_Img[idx])     
		m_Target_Img[idx]->visible = false;

	if (m_OrbShadows[idx])  
		m_OrbShadows[idx]->visible = false;

	if (m_Target_Orb[idx])   
		m_Target_Orb[idx]->visible = false;

	m_Target_Img[idx].reset();
	m_OrbShadows[idx].reset();
	m_Target_Orb[idx].reset();

	m_OrbStates[idx].dead = true;
}

void CTarget_Orb::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit) return;

	CBossPawn* boss = dynamic_cast<CBossPawn*>(hit);
	if (boss) return;

	if (hit == m_Skill_Info.Instigator) return;

	if (m_HitObjects.find(other) != m_HitObjects.end())
		return;

	m_HitObjects.insert(other);

	if (hit == m_Skill_Info.Target)
	{
		if (hit->IsKnockback())
		{
			Vec2 dir = hit->Get_WorldPos() - m_StartPos;
			dir.Normalize();
			hit->Get_Collision()->MoveWorldPos(dir * m_Skill_Info.Amount * DELTA);
		}

		FHitDamage hitDamage;
		hitDamage.Damage = m_Skill_Info.Damage;
		hitDamage.colldier = collider;
		hitDamage.other = other;

		hit->TakeDamage(hitDamage);
	}

	cout << "To Spear Hit" << endl;
}

void CTarget_Orb::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);
}

void CTarget_Orb::OnHitBySpear(GameObject* collider)
{
	for (int i = 0; i < m_Target_Orb.size(); ++i)
	{
		if (!m_Target_Orb[i]) continue;

		if (m_Target_Orb[i].get() == collider)
		{
			if (m_OrbStates[i].isArrived == false)
				return;

			SOUND->StopPlay("ImpactLightning", 0.1f);

			PARTICLE->Spawn_Particle(L"ShockSpark_AS", m_Target_Orb[i]->GetWorldPos(), Vec2(1.f, 1.5f));

			m_OrbStates[i].isHit = true;
			m_OrbStates[i].boomTimer = 0.3f;

			Vec2 size = Vec2(32.f, 28.f) * 3.f;
			shared_ptr<ObImage> hitSprite = RESOURCE->CreateImage(L"Monster\\Lightning\\LightningBossTargetCharged.png", size);
			hitSprite->SetParentRT(*m_Target_Orb[i]);
			hitSprite->SetLocalPosY(0.f);

			m_Target_Img[i] = hitSprite;
			m_Target_Img[i]->visible = true;

			break;
		}
	}
}

void CTarget_Orb::RecreateOrb(int i)
{
	m_OrbStates[i] = FOrbState{}; // 상태 초기화

	auto circle = make_shared<ObCircle>();
	circle->scale = Vec2(50.f, 50.f);
	circle->isFilled = false;
	circle->visible = true;
	circle->collider = COLLIDER::CIRCLE;
	circle->Set_Owner(this);
	circle->SetParentRT(*m_Skill_Info.Instigator->Get_Collision());
	circle->SetLocalPos(Vec2(cosf(m_Angle[i]) * m_Radius, sinf(m_Angle[i]) * m_Radius));
	COLLISION->AddCollider(circle.get());
	m_Target_Orb[i] = circle;

	auto idleImg = RESOURCE->CreateImage(L"Monster\\Lightning\\LightningBossTargetIdle.png", m_OrbSize, true);
	idleImg->SetParentRT(*m_Target_Orb[i]);
	idleImg->SetLocalPosY(20.f);
	idleImg->visible = true;
	m_Target_Img[i] = idleImg;

	auto shadow = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(75.f), true);
	shadow->SetParentRT(*m_Target_Orb[i]);
	shadow->color = Color(0.5f, 0.5f, 0.5f, 0.25f);
	shadow->visible = true;
	m_OrbShadows[i] = shadow;

	m_OrbStates[i].dead = false;
}