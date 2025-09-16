#include "stdafx.h"
#include "CFireBall.h"
#include "CResourceManager.h"

CFireBall::CFireBall(SkillInfo& _Info)
{
	m_Damage = _Info.Damage;
	m_Pos = _Info.SpawnPos;
	m_MoveDir = _Info.Dir;

	m_FireBall_Col = make_shared<ObCircle>();
	m_FireBall_Col->scale = Vec2(100.f, 100.f);
	m_FireBall_Col->SetWorldPos(Vec2(m_Pos));
	m_FireBall_Col->rotation = PI/2;
	m_FireBall_Col->visible = true;
	m_FireBall_Col->isFilled = false;
	m_FireBall_Col->collider = COLLIDER::CIRCLE;

	m_FireBall = RESOURCE->CreateSprite(L"Skill\\SKILL_FIREDRAGON_RIGHT.png", 
		Vec2(117.f, 179.f) * 0.7f, Int2(1,1), true);

	m_FireBall->SetParentRT(*m_FireBall_Col);

}

CFireBall::~CFireBall()
{

}

void CFireBall::BeginPlay()
{
	CActor::BeginPlay();

}

void CFireBall::Tick()
{
	CActor::Tick();

	if (m_FireBall_Col)
		m_Pos = m_FireBall_Col->GetWorldPos();

	m_FireBall_Col->MoveWorldPos(m_MoveDir * 800.f * DELTA);

	m_FireBall_Col->Update();
	m_FireBall->Update();
}

void CFireBall::LateUpdate()
{
	CActor::LateUpdate();

}

void CFireBall::Render()
{
	CActor::Render();

	m_FireBall_Col->Render();
	m_FireBall->Render();
}

void CFireBall::Release()
{
	CActor::Release();


}
