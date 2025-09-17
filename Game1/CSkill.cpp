#include "stdafx.h"
#include "CSkill.h"

#include "CFireBall.h"

CSkill::CSkill()
{

}

CSkill::~CSkill()
{

}

void CSkill::BeginPlay()
{

}

void CSkill::Tick()
{

}

void CSkill::Render()
{

}

bool CSkill::CanActivate(const SkillInfo& _Info)
{
	// TODO : _Info 구조체 점점 제대로 세팅시켜주기
	//if (_Info.Owner == nullptr) return false;
	// if (_Info.Aim_Dir.LengthSquared) // 방향 0 방지 추가하기
	// if(_Info.Mp > Owner->StatusComponent 가셔와서 마나 비교 후 false)

	return true;
}

void CSkill::Activate(CActor* _Owner)
{
	
}
