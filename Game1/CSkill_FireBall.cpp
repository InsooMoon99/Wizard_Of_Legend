#include "stdafx.h"
#include "CSkill_FireBall.h"
#include "CFire_Ball.h"
#include "CCharacter.h"
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

	m_Id = "FireBall";

	if (auto data = SKILL_DATA->GetById(m_Id))
	{
		m_DataDamage = data->Damage;
		m_DataCoolDown = data->CoolDown - 0.1f;
	}
	else
	{
		m_DataDamage = 10.f;
		m_DataCoolDown = 5.f - 0.1f;
	}

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

void CSkill_FireBall::LateUpdate()
{
	CSkill::LateUpdate();

}

void CSkill_FireBall::Release()
{
	CSkill::Release();

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

bool CSkill_FireBall::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;

	// 부모쪽 CSkill에서 CoolRemain으로 처리

	return true;
}

void CSkill_FireBall::Activate(CActor* _Owner)
{
	if (!CanActivate(m_Skill_Info)) return;

	CSkill::Activate(_Owner);

	CCharacter* character = dynamic_cast<CCharacter*>(_Owner);
	if (!character) return;
	
	auto state = CHelper::GetComponent<CStateComponent>(character);
	if (!state) return;

	state->Change_State(state->Set_AttackType());

	Vec2 dir = CHelper::DirToVec(state->Get_Dir());
	float radian = Utility::DirToRadian(dir);

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Aim_Dir = dir;
	m_Skill_Info.Angle = radian;
	m_Skill_Info.Damage = m_DataDamage;
	m_Skill_Info.Amount = 1000.f;

	auto fireBall = make_shared<CFire_Ball>(m_Skill_Info);
	fireBall->BeginPlay();

	m_FireBall.push_back(fireBall);

	StartCoolDown();
}
