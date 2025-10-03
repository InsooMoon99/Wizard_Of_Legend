#include "stdafx.h"
#include "CMiddle_Fire.h"

#include "CBossPawn.h"
#include "CEarth_Shield.h"
#include "CStatusComponent.h"

CMiddle_Fire::CMiddle_Fire(FSkillInfo& _Info)
	: CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = _Info.SpawnPos;

	m_Collision = make_shared<ObCircle>();
	m_Collision->scale = Vec2(80.f, 80.f);
	m_Collision->SetWorldPos(m_Pos);
	m_Collision->visible = false;
	m_Collision->collider = COLLIDER::CIRCLE;
	m_Collision->isFilled = false;
	m_Collision->Set_Owner(this);
	m_Collision->Set_Instigator(m_Skill_Info.Instigator);

	COLLISION->AddCollider(m_Collision.get());

	Vec2 size = Vec2(276.f / 6.f, 45.f) * 2.f;
	m_FireBall = RESOURCE->CreateSprite(L"Monster\\Wizard\\Wizard_Fire.png", size, Int2(6, 1), true);
	m_FireBall->SetParentRT(*m_Collision);
	m_FireBall->ChangeAnim(ANIMSTATE::LOOP, 0.1f);

}

CMiddle_Fire::~CMiddle_Fire()
{
	Release();
}

void CMiddle_Fire::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	if (m_Skill_Info.Aim_Dir.LengthSquared() > 1e-6f)
	{
		m_Dir = m_Skill_Info.Aim_Dir;
		m_Dir.Normalize();
	}

	else
	{
		Vec2(1.f, 0.f);
	}
}

void CMiddle_Fire::Tick()
{
	CSkill_Actor::Tick();

	if (m_PendingKill) return;

	// 위치 갱신
	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	m_LifeTime += DELTA;

	if (m_LifeTime >= 3.f)
	{
		m_PendingKill = true;
	}

	Vec2 delta = m_Dir * 300.f * DELTA;

	if (m_Collision) 
	{
		m_Collision->MoveWorldPos(delta);
		m_Pos = m_Collision->GetWorldPos();
	}

	m_Collision->Update();
	m_FireBall->Update();
}

void CMiddle_Fire::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CMiddle_Fire::Render()
{
	CSkill_Actor::Render();

	if (m_PendingKill) return;

	m_Collision->Render();
	m_FireBall->Render();
}

void CMiddle_Fire::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
}

void CMiddle_Fire::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	auto ownerObj = other ? other->Get_Owner() : nullptr;

	if (ownerObj && dynamic_cast<CEarth_Shield*>(ownerObj))
	{
		Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
		Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
		PARTICLE->Spawn_ParticleDir(L"HitSpark", Vec2(particlePos) + randomVec);
		SOUND->StopPlay("ImpactEarth", 0.2f);

		m_PendingKill = true;

		return;
	}


	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	if (hit == m_Skill_Info.Instigator) return;

	FHitDamage hitDamage;
	{
		Vec2 dir = hit->Get_WorldPos() - m_StartPos;
		if (dir.LengthSquared() > 1e-8f) dir.Normalize();

		hitDamage.Dir = dir;
		hitDamage.Damage = m_Skill_Info.Damage;
		hitDamage.colldier = collider;
		hitDamage.other = other;

		if (hit->IsKnockbackPrev())
		{
			hitDamage.Amount = m_Skill_Info.Amount;
			hitDamage.Duration = 0.25f;
		}
		else
		{
			hitDamage.Amount = 0.f;
			hitDamage.Duration = 0.f;
		}
	}

	hit->TakeDamage(hitDamage);

	m_PendingKill = true;
	
}

void CMiddle_Fire::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}