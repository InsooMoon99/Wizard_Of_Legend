#include "stdafx.h"
#include "CSkill_Earth_Shield.h"
#include "CCharacter.h"
#include "CEarth_Shield.h"
#include "CStateComponent.h"

CSkill_Earth_Shield::CSkill_Earth_Shield()
{
}

CSkill_Earth_Shield::~CSkill_Earth_Shield()
{
}

void CSkill_Earth_Shield::BeginPlay()
{
	CSkill::BeginPlay();

	m_Id = "EarthShield";

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

void CSkill_Earth_Shield::Tick()
{
	CSkill::Tick();

	for (auto iter = m_Armor.begin(); iter != m_Armor.end(); )
	{
		auto armor = *iter;
		armor->Tick();

		if (armor->IsDestroy())
		{
			armor->Release();
			iter = m_Armor.erase(iter);
		}

		else
			++iter;
	}

}

void CSkill_Earth_Shield::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_Earth_Shield::Render()
{
	CSkill::Render();

	for (auto& armor : m_Armor)
	{
		if (!armor->IsDestroy())
			armor->Render();
	}
}

void CSkill_Earth_Shield::Release()
{
	CSkill::Release();
}

bool CSkill_Earth_Shield::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))

		// TODO : 나중에 스킬 쿨타임에 따라서 판단

		return true;
}

void CSkill_Earth_Shield::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

	CCharacter* character = dynamic_cast<CCharacter*>(_Owner);
	if (!character) return;

	auto state = CHelper::GetComponent<CStateComponent>(character);
	if (!state) return;

	// TODO : 다른 포즈로 변경 예정
	state->Change_State(state->Set_AttackType());

	Vec2 dir = CHelper::DirToVec(state->Get_Dir());
	float radian = Utility::DirToRadian(dir);

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Aim_Dir = dir;
	m_Skill_Info.Angle = radian;
	m_Skill_Info.Damage = m_DataDamage;
	m_Skill_Info.Amount = 200.f;

	auto wind = make_shared<CEarth_Shield>(m_Skill_Info);
	wind->BeginPlay();

	m_Armor.push_back(wind);

	StartCoolDown();
}
