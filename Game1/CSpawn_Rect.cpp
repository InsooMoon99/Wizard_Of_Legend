#include "stdafx.h"
#include "CSpawn_Rect.h"

CSpawn_Rect::CSpawn_Rect(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
}

CSpawn_Rect::~CSpawn_Rect()
{

}

void CSpawn_Rect::BeginPlay()
{
	CSkill_Actor::BeginPlay();
}

void CSpawn_Rect::Tick()
{
	CSkill_Actor::Tick();
}

void CSpawn_Rect::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CSpawn_Rect::Render()
{
	CSkill_Actor::Render();
}

void CSpawn_Rect::Release()
{
	CSkill_Actor::Release();
}

bool CSpawn_Rect::CheckHitWall()
{
	return CSkill_Actor::CheckHitWall();
}

void CSpawn_Rect::OnHitWall(const Vec2& hitPos)
{
	CSkill_Actor::OnHitWall(hitPos);
}

void CSpawn_Rect::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);
}

void CSpawn_Rect::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);
}
