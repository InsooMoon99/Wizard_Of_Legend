#include "stdafx.h"
#include "CStar_Collision.h"

#include "CPlayer.h"

CStar_Collision::CStar_Collision(FSkillInfo& _Info, EStarType _Type) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	{
		m_Collision = make_shared<ObRect>();
		//m_Collision->scale = Vec2(190.f, 15.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x, m_Pos.y));
		m_Collision->visible  = false;
		m_Collision->isFilled = false;
		m_Collision->rotation = _Info.Angle;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);
		m_Collision->pivot = OFFSET_L;
		m_Collision->collider = COLLIDER::RECT;

		COLLISION->AddCollider(m_Collision.get());
	}

	Vec2 size;
	if (_Type == EStarType::First)
	{
		size = Vec2(112.f / 7.f, 64.f * 3.f) * 1.5f;
		m_Collision->scale = Vec2(190.f, 10.f);
	}

	else if (_Type == EStarType::Second)
	{
		size = Vec2(112.f / 7.f, 64.f * 6.f) * 1.5f;
		m_Collision->scale = Vec2(330.f, 10.f);
	}

	else if (_Type == EStarType::Third)
	{
		size = Vec2(112.f / 7.f, 64.f * 4.f) * 1.5f;
		m_Collision->scale = Vec2(240.f, 10.f);
	}

	
	m_Neddle = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_Neddle_Loop.png", size, Int2(7, 1), true);
	m_Neddle->SetParentRT(*m_Collision);
	m_Neddle->SetLocalPos(Vec2(70.f, 0.f));
	m_Neddle->rotation -= (PI / 2);
	m_Neddle->ChangeAnim(ANIMSTATE::LOOP, 0.1f);



}

CStar_Collision::~CStar_Collision()
{
	Release();
}

void CStar_Collision::BeginPlay()
{
	CSkill_Actor::BeginPlay();


}

void CStar_Collision::Tick()
{
	CSkill_Actor::Tick();

	m_Collision->Update();
	m_Neddle->Update();
}

void CStar_Collision::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CStar_Collision::Render()
{
	CSkill_Actor::Render();

	m_Collision->Render();
	m_Neddle->Render();
}

void CStar_Collision::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
}

void CStar_Collision::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CPlayer* hit = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!hit) return;

	if (m_HitPlayer.find(hit) != m_HitPlayer.end())
		return;

	m_HitPlayer.insert(hit);
	
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
	

	cout << "충돌 ? " << endl;

	bCheck = true;

	cout << bCheck << endl;
}

void CStar_Collision::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

	bCheck = false;
}
