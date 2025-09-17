#include "stdafx.h"
#include "CFireBall.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CStatusComponent.h"

CFireBall::CFireBall(SkillInfo& _Info)
{
	m_Skill_Info = _Info;
	m_Pos = _Info.SpawnPos;

	// FireBall Collision
	{
		m_FireBall_Col = make_shared<ObCircle>();
		m_FireBall_Col->scale = Vec2(100.f, 100.f);
		m_FireBall_Col->SetWorldPos(Vec2(m_Pos.x, m_Pos.y));
		m_FireBall_Col->visible = true;
		m_FireBall_Col->collider = COLLIDER::CIRCLE;
		m_FireBall_Col->isFilled = false;
		m_FireBall_Col->rotation = m_Skill_Info.Angle;
		m_FireBall_Col->Set_Owner(this);
		m_FireBall_Col->Set_Instigator(m_Skill_Info.Instigator);

		COLLISION->AddCollider(m_FireBall_Col.get());
	}

	m_FireBall = RESOURCE->CreateSprite(L"Skill\\FireBall.png", Vec2(980.f / 14.f, 50.f) * 2.5f, Int2(14,1), true);
	m_FireBall->SetParentRT(*m_FireBall_Col);
	m_FireBall->ChangeAnim(ANIMSTATE::ONCE, 0.05f);

}

CFireBall::~CFireBall()
{
	Release();
}

void CFireBall::BeginPlay()
{
	Super::BeginPlay();

}

void CFireBall::Tick()
{
	Super::Tick();

	if (m_PendingKill)
		return;

	if (m_FireBall_Col)
		m_Pos = m_FireBall_Col->GetWorldPos();

	m_SpawnLife += DELTA;

	if (m_SpawnLife >= 1.5f)
	{
		m_PendingKill = true;
		return;
	}

	m_FireBall_Col->MoveWorldPos(m_Skill_Info.Aim_Dir * 800.f * DELTA);

	m_FireBall_Col->Update();
	m_FireBall->Update();
}

void CFireBall::LateUpdate()
{
	Super::LateUpdate();

	
}

void CFireBall::Render()
{
	Super::Render();

	if (m_PendingKill) return;

	m_FireBall_Col->Render();
	m_FireBall->Render();
}

void CFireBall::Release()
{
	Super::Release();

	if (m_FireBall_Col)
	{             
		COLLISION->RemoveCollider(m_FireBall_Col.get());

		m_FireBall_Col.reset();
	}
	m_FireBall.reset();
}

void CFireBall::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentBeginOverlap(collider, other);

	// 일단, Character랑만 충돌이 되도록 음.. 벽이랑도 충돌이 되네
	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;
	
	// 팀킬/자기 무시
	if (hit == m_Skill_Info.Instigator) return;
	//if (IsSameTeam(hit, m_FireBall_Col->Get_Instigator())) return;

	auto* status = CHelper::GetComponent<CStatusComponent>(hit);
	if (!status) return;
	
	status->SetHp(status->GetHp() - m_Skill_Info.Damage);

	Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
	PARTICLE->Spawn_Particle(L"Particle", particlePos);

	m_PendingKill = true;
}

void CFireBall::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);


}
