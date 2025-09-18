#include "stdafx.h"
#include "CFireBall.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CStatusComponent.h"

CFireBall::CFireBall(SkillInfo& _Info)
{
	m_Skill_Info = _Info;
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

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

	// 위치 갱신
	if (m_FireBall_Col)
		m_Pos = m_FireBall_Col->GetWorldPos();

	// Life Time
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

	// TODO : 벽이랑도 충돌되도록 타입 변경
	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	// TODO : 팀킬/자기 무시
	if (hit == m_Skill_Info.Instigator) return;
	//if (IsSameTeam(hit, m_FireBall_Col->Get_Instigator())) return;

	Vec2 dir = hit->Get_WorldPos() - m_StartPos;
	dir.Normalize();
	hit->Get_Collision()->MoveWorldPos(dir * m_Skill_Info.Amount * DELTA);

	// 색 변경
	IColorChanger* colorChanger = dynamic_cast<IColorChanger*>(hit);
	if (colorChanger)
		colorChanger->Hit_Color();

	auto* status = CHelper::GetComponent<CStatusComponent>(hit);
	if (!status) return;
	
	status->SetHp(status->GetHp() - m_Skill_Info.Damage);

	// Particle
	Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
	PARTICLE->Spawn_Particle(L"HitSpark", particlePos);
	PARTICLE->Spawn_Particle(L"Fire_HitEffect", other->GetWorldPos());

	m_PendingKill = true;
}

void CFireBall::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);


}
