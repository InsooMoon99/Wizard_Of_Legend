#include "stdafx.h"
#include "CSkill_FireBall.h"

#include "CFireBall.h"
#include "CPlayer.h"
#include "CStateComponent.h"

CSkill_FireBall::CSkill_FireBall()
{
	

}

CSkill_FireBall::~CSkill_FireBall()
{

}

void CSkill_FireBall::BeginPlay()
{
	CSkill::BeginPlay();

	
}

void CSkill_FireBall::Tick()
{
	CSkill::Tick();

	for (auto iter = m_FireBall.begin(); iter != m_FireBall.end(); )
	{
		auto fireball = *iter;

		fireball->Tick();

		if (fireball->IsDestroy())
			iter = m_FireBall.erase(iter);

		else
			++iter;
	}

}

void CSkill_FireBall::Render()
{
	CSkill::Render();

	for (auto& fireBall : m_FireBall) 
	{
		if (!fireBall->IsDestroy())
			fireBall->Render();
	}
}

bool CSkill_FireBall::CanActivate(const SkillInfo& _Skill)
{
	if (!CSkill::CanActivate(_Skill))
		return false;

	// TODO : 나중에 스킬 쿨타임에 따라서 판단

	return true;
}

void CSkill_FireBall::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

	CPlayer* player = dynamic_cast<CPlayer*>(_Owner);
	if (!player) return;

	auto* state = CHelper::GetComponent<CStateComponent>(player);
	if (!state) return;

	Vec2 dir = CHelper::DirToVec(state->Get_Dir());
	float radian = Utility::DirToRadian(dir);

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Aim_Dir = dir;
	m_Skill_Info.Angle = radian;
	m_Skill_Info.Damage = 10.f;

	//m_Skill_Info.Amount = Aim_Dir 방향값 * 넉백수치로 세팅;

	auto fireBall = make_shared<CFireBall>(m_Skill_Info);
	fireBall->BeginPlay();

	m_FireBall.push_back(fireBall);
}
