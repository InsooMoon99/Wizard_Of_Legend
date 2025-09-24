#include "stdafx.h"
#include "CStar.h"

CStar::CStar(FSkillInfo& _Info) : CSkill_Actor(_Info)
{

}

CStar::~CStar()
{
}

void CStar::BeginPlay()
{
	CSkill_Actor::BeginPlay();
}

void CStar::Tick()
{
	CSkill_Actor::Tick();
}

void CStar::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CStar::Render()
{
	CSkill_Actor::Render();
}

void CStar::Release()
{
	CSkill_Actor::Release();
}

void CStar::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);
}

void CStar::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);
}
