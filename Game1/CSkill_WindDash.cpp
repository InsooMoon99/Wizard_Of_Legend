#include "stdafx.h"
#include "CSkill_WindDash.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CWindSlash.h"

CSkill_WindDash::CSkill_WindDash()
{
}

CSkill_WindDash::~CSkill_WindDash()
{
}

void CSkill_WindDash::BeginPlay()
{
	CSkill::BeginPlay();

	m_Id = "WindDash";

	if (auto data = SKILL_DATA->GetById(m_Id))
	{
		m_DataDamage = data->Damage;
		m_DataCoolDown = data->CoolDown;
	}
	else
	{
		m_DataDamage = 10.f;
		m_DataCoolDown = 5.f;
	}
}

void CSkill_WindDash::Tick()
{
	CSkill::Tick();

	
}

void CSkill_WindDash::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_WindDash::Render()
{
	CSkill::Render();

	
}

void CSkill_WindDash::Release()
{
	CSkill::Release();
}

bool CSkill_WindDash::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))

	// TODO : 나중에 스킬 쿨타임에 따라서 판단

	return true;

}

void CSkill_WindDash::Activate(CActor* _Owner)
{
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

	StartCoolDown();
}
