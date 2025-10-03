#include "stdafx.h"
#include "CJavelin_Spear.h"

#include "CBossPawn.h"
#include "CBoss_TwinGirl.h"
#include "CBoss_Controller.h"
#include "CTarget_Orb.h"

CJavelin_Spear::CJavelin_Spear(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	// 충돌체
	{
		m_Collision = make_shared<ObCircle>();
		m_Collision->scale = Vec2(100.f, 60.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x, m_Pos.y));
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::CIRCLE;
		m_Collision->isFilled = false;
		m_Collision->rotation = -m_Skill_Info.Angle;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);
		m_Collision->pivot = OFFSET_N;

		COLLISION->AddCollider(m_Collision.get());

		//m_Collision->rotation += PI / 2;
	}

	Vec2 spearSize = Vec2(144.f / 3.f, 39.f) * 3.5f;
	m_Spear = RESOURCE->CreateSprite(L"Monster\\Lightning\\JavelinSpear.png", spearSize, Int2(3, 1), true);
	m_Spear->SetParentRT(*m_Collision);
	m_Spear->SetLocalPos(Vec2(26.f, 12.f));
	m_Spear->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
}

CJavelin_Spear::~CJavelin_Spear()
{
	Release();
}

void CJavelin_Spear::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	m_Boss = dynamic_cast<CBoss_TwinGirl*>(m_Skill_Info.Instigator);
	m_BossController = dynamic_cast<CBossPawn*>(m_Skill_Info.Instigator)->GetController();

	m_AttackPos = Vec2(m_Boss->GetJavelinPos().x, m_Boss->GetJavelinPos().y);

}

void CJavelin_Spear::Tick()
{
	CSkill_Actor::Tick();

	Vec2 targetCenterPos = m_AttackPos + Vec2(0.f, m_Collision->scale.y / 2.f);

	m_LifeTime += DELTA;

	if (m_LifeTime >= 0.7f)
	{
		m_PendingKill = true;

		return;

	}

	Vec2 toTarget = targetCenterPos - m_Collision->GetWorldPos();
	float dist = toTarget.Length();
	float speed = 1600.f;
	float step = speed * DELTA;

	float moveDistance = min(dist, step);

	if (dist <= step)
	{
		m_Collision->SetWorldPos(targetCenterPos);
	}
	else
	{
		toTarget.Normalize();
		m_Collision->MoveWorldPos(toTarget * moveDistance);
	}

	if (bReleaseCheck == true)
	{
		m_ReleaseTime -= DELTA;

		if (m_ReleaseTime <= 0.f)
		{
			m_PendingKill = true;
		}
	}

	m_Collision->Update();
	m_Spear->Update();
}

void CJavelin_Spear::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CJavelin_Spear::Render()
{
	CSkill_Actor::Render();

	m_Collision->Render();
	m_Spear->Render();
}

void CJavelin_Spear::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}

	//m_Spear.reset();
}

void CJavelin_Spear::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CTarget_Orb* targetOrb = dynamic_cast<CTarget_Orb*>(other->Get_Owner());

	if (targetOrb)
	{
		targetOrb->OnHitSpear();
		//m_PendingKill = true;
		//COLLISION->RemoveCollider(m_Collision.get());
		bReleaseCheck = true;
		m_ReleaseTime = 0.3f;

		return;
	}

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
		if (hit->IsKnockbackPrev())
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
}

void CJavelin_Spear::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{  
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}

