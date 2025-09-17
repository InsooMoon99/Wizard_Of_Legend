#include "stdafx.h"
#include "CSkill_Actor.h"

CSkill_Actor::CSkill_Actor()
{
}

CSkill_Actor::~CSkill_Actor()
{
}

void CSkill_Actor::BeginPlay()
{
	CActor::BeginPlay();
}

void CSkill_Actor::Tick()
{
	CActor::Tick();
}

void CSkill_Actor::LateUpdate()
{
	CActor::LateUpdate();
}

void CSkill_Actor::Render()
{
	CActor::Render();
}

void CSkill_Actor::Release()
{
	CActor::Release();
}

void CSkill_Actor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CActor::OnComponentBeginOverlap(collider, other);
}

void CSkill_Actor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CActor::OnComponentEndOverlap(collider, other);
}
