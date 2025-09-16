#include "stdafx.h"
#include "CSkill_FireBall.h"

#include "CFireBall.h"

CSkill_FireBall::CSkill_FireBall()
{
	// TODO : 일단은 데미지 세팅만, 아직 충돌처리 안만듦
	// 쿨타임 Setting도 추가하기

	m_Skill_Info.Damage = 10.f;
}

CSkill_FireBall::~CSkill_FireBall()
{

}

void CSkill_FireBall::BeginPlay()
{
	CSkill::BeginPlay();

	if (m_FireBall)
		m_FireBall->BeginPlay();
}

void CSkill_FireBall::Tick()
{
	CSkill::Tick();

	if (m_FireBall)
		m_FireBall->Tick();
}

void CSkill_FireBall::Render()
{
	CSkill::Render();

	if (m_FireBall)
		m_FireBall->Render();
}

bool CSkill_FireBall::CanActivate(const SkillInfo& _Skill)
{
	if (!CSkill::CanActivate(_Skill))
		return false;


	return true;
}

void CSkill_FireBall::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

	m_Skill_Info.Owner = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Dir = Vec2(1.0f, 0.0f);

	m_FireBall = make_shared<CFireBall>(m_Skill_Info);

}
