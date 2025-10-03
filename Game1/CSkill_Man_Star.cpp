#include "stdafx.h"
#include "CSkill_Man_Star.h"

CSkill_Man_Star::CSkill_Man_Star()
{
}

CSkill_Man_Star::~CSkill_Man_Star()
{
}

void CSkill_Man_Star::BeginPlay()
{
	CSkill::BeginPlay();
}

void CSkill_Man_Star::Tick()
{
	CSkill::Tick();
}

void CSkill_Man_Star::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_Man_Star::Render()
{
	CSkill::Render();
}

void CSkill_Man_Star::Release()
{
	CSkill::Release();
}

bool CSkill_Man_Star::CanActivate(const FSkillInfo& _Info)
{
	return CSkill::CanActivate(_Info);
}

void CSkill_Man_Star::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

}
