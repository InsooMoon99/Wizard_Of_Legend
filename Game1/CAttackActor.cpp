#include "stdafx.h"
#include "CAttackActor.h"

#include "CPlayer.h"

CAttackActor::CAttackActor(FSkillInfo& _Info, const Vec2& Size) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	{
		m_Collision = make_shared<ObRect>();
		m_Collision->scale = Size;
		m_Collision->SetWorldPos(m_Pos);
		m_Collision->visible  = true;
		m_Collision->isFilled = false;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);
		m_Collision->pivot = OFFSET_N;
		m_Collision->collider = COLLIDER::RECT;

		COLLISION->AddCollider(m_Collision.get());
	}

	m_Lifetime = 0.f;
}

CAttackActor::~CAttackActor()
{
	Release();
}

void CAttackActor::BeginPlay()
{
	CSkill_Actor::BeginPlay();

}

void CAttackActor::Tick()
{
	CSkill_Actor::Tick();

	if (m_PendingKill) return;

	m_Lifetime += DELTA;

	if (m_Lifetime >= 0.5f)
	{
		m_PendingKill = true;
	}

	m_Collision->Update();
}

void CAttackActor::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CAttackActor::Render()
{
	CSkill_Actor::Render();

	m_Collision->Render();
}

void CAttackActor::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
}

void CAttackActor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	if (m_HitPlayer.find(player) != m_HitPlayer.end())
		return;

	m_HitPlayer.insert(player);

	FHitDamage hitDamage;

	Vec2 dir = player->Get_WorldPos() - m_Collision->GetWorldPos();
	if (dir.LengthSquared() > 1e-8f)
		dir.Normalize();

	hitDamage.Dir = dir;

	hitDamage.Damage = m_Skill_Info.Damage;
	hitDamage.colldier = collider;
	hitDamage.other = other;
	hitDamage.Amount = 700.f;
	hitDamage.Duration = 0.25;
	player->TakeDamage(hitDamage);
	
	m_PendingKill = true;
}

void CAttackActor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}
