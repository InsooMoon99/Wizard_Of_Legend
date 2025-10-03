#include "stdafx.h"
#include "CMiddle_SpawnBall.h"
#include "CBossPawn.h"
#include "CStatusComponent.h"
#include "CEarth_Shield.h"

CMiddle_SpawnBall::CMiddle_SpawnBall(FSkillInfo& _Info)
	: CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = _Info.SpawnPos;

	Vec2 pos_Offset = m_Skill_Info.Aim_Dir * 80.f;
	float rotation_Offset = - PI / 2;

	Vec2 pos = Vec2(m_Pos.x + pos_Offset.x, m_Pos.y + pos_Offset.y);

	m_Collision = make_shared<ObCircle>();
	m_Collision->scale = Vec2(80.f, 80.f);
	m_Collision->SetWorldPos(pos);
	m_Collision->visible = false;
	m_Collision->collider = COLLIDER::CIRCLE;
	m_Collision->isFilled = false;
	m_Collision->rotation = m_Skill_Info.Angle + rotation_Offset;
	m_Collision->Set_Owner(this);
	m_Collision->Set_Instigator(m_Skill_Info.Instigator);

	COLLISION->AddCollider(m_Collision.get());

	Vec2 size = Vec2(41.f, 39.f) * 2.f;
	m_SpawnBall = RESOURCE->CreateImage(L"Monster\\Wizard\\WIZARD_BALL _0.png", size, true);
	m_SpawnBall->SetParentRT(*m_Collision);
}

CMiddle_SpawnBall::~CMiddle_SpawnBall()
{
	Release();
}

void CMiddle_SpawnBall::BeginPlay()
{
	CSkill_Actor::BeginPlay();


}

void CMiddle_SpawnBall::Tick()
{
	CSkill_Actor::Tick();
	if (m_PendingKill) return;


	m_LifeTime += DELTA;

	if (m_LifeTime >= 3.f)
		//m_PendingKill = true;


	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	m_Collision->Update();

	if(m_SpawnBall)
		m_SpawnBall->Update();
}


void CMiddle_SpawnBall::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CMiddle_SpawnBall::Render()
{
	CSkill_Actor::Render();

	if (m_PendingKill) return;

	m_Collision->Render();
	m_SpawnBall->Render();
}

void CMiddle_SpawnBall::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
}

void CMiddle_SpawnBall::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	if (hit == m_Skill_Info.Instigator) return;
	//if (IsSameTeam(hit, m_Collision->Get_Instigator())) return;

	IColorChanger* colorChanger = hit;
	if (colorChanger)
		colorChanger->Hit_Color();

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

	//m_PendingKill = true;
}

void CMiddle_SpawnBall::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}