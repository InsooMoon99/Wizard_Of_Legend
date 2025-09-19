#include "stdafx.h"
#include "CSkill_Gaia_Armor.h"
#include "CCharacter.h"
#include "CGaia_Armor.h"
#include "CStateComponent.h"

CSkill_Gaia_Armor::CSkill_Gaia_Armor()
{
}

CSkill_Gaia_Armor::~CSkill_Gaia_Armor()
{
}

void CSkill_Gaia_Armor::BeginPlay()
{
	CSkill::BeginPlay();
}

void CSkill_Gaia_Armor::Tick()
{
	CSkill::Tick();

	for (auto iter = m_Armor.begin(); iter != m_Armor.end(); )
	{
		auto armor = *iter;

		armor->Tick();

		if (armor->IsDestroy())
			iter = m_Armor.erase(iter);

		else
			++iter;
	}
}

void CSkill_Gaia_Armor::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_Gaia_Armor::Render()
{
	CSkill::Render();

	for (auto& armor : m_Armor)
	{
		if (!armor->IsDestroy())
			armor->Render();
	}
}

void CSkill_Gaia_Armor::Release()
{
	CSkill::Release();
}

bool CSkill_Gaia_Armor::CanActivate(const SkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))

		// TODO : 나중에 스킬 쿨타임에 따라서 판단

		return true;
}

void CSkill_Gaia_Armor::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

	CCharacter* character = dynamic_cast<CCharacter*>(_Owner);
	if (!character) return;

	auto* state = CHelper::GetComponent<CStateComponent>(character);
	if (!state) return;

	Vec2 dir = CHelper::DirToVec(state->Get_Dir());
	float radian = Utility::DirToRadian(dir);

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Aim_Dir = dir;
	m_Skill_Info.Angle = radian;
	m_Skill_Info.Damage = 10.f;
	m_Skill_Info.Amount = 1000.f;

	auto wind = make_shared<CGaia_Armor>(m_Skill_Info);
	wind->BeginPlay();

	m_Armor.push_back(wind);
}
