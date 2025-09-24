#include "stdafx.h"
#include "CSkill_Girl_Star.h"

CSkill_Girl_Star::CSkill_Girl_Star()
{
}

CSkill_Girl_Star::~CSkill_Girl_Star()
{
}

void CSkill_Girl_Star::BeginPlay()
{
	CSkill::BeginPlay();
}

void CSkill_Girl_Star::Tick()
{
	CSkill::Tick();
}

void CSkill_Girl_Star::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_Girl_Star::Render()
{
	CSkill::Render();
}

void CSkill_Girl_Star::Release()
{
	CSkill::Release();
}

bool CSkill_Girl_Star::CanActivate(const FSkillInfo& _Info)
{
	return CSkill::CanActivate(_Info);
}

void CSkill_Girl_Star::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);
}
