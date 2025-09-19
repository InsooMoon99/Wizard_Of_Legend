#include "stdafx.h"
#include "CWindSlash.h"
#include "CStatusComponent.h"

CWindSlash::CWindSlash(SkillInfo& _Info)
	: CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = _Info.SpawnPos;

	Vec2 pos_Offset = m_Skill_Info.Aim_Dir * 50.f;
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

	shared_ptr<ObImage> wind = PARTICLE->Spawn_Particle(L"Wind_Slash", pos);
	wind->rotation = m_Collision->rotation;
}

CWindSlash::~CWindSlash()
{
	Release();
}

void CWindSlash::BeginPlay()
{
	CSkill_Actor::BeginPlay();


}

void CWindSlash::Tick()
{
	CSkill_Actor::Tick();

	if (m_PendingKill) return;

	// 위치 갱신
	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();


	m_Collision->Update();
}

void CWindSlash::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CWindSlash::Render()
{
	CSkill_Actor::Render();

	if (m_PendingKill) return;

	m_Collision->Render();
}

void CWindSlash::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
}

void CWindSlash::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	if (hit == m_Skill_Info.Instigator) return;
	//if (IsSameTeam(hit, m_Collision->Get_Instigator())) return;

	Vec2 dir = hit->Get_WorldPos() - m_StartPos;
	dir.Normalize();
	hit->Get_Collision()->MoveWorldPos(dir * m_Skill_Info.Amount * DELTA);

	IColorChanger* colorChanger = hit;
	if (colorChanger)
		colorChanger->Hit_Color();

	auto* status = CHelper::GetComponent<CStatusComponent>(hit);
	if (status)
		status->SetHp(status->GetHp() - m_Skill_Info.Damage);

	Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
	Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
	PARTICLE->Spawn_Particle(L"HitSpark", Vec2(particlePos) + randomVec);

	m_PendingKill = true;
}

void CWindSlash::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);
}
