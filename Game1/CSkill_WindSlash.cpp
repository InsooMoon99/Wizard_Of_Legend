#include "stdafx.h"
#include "CSkill_WindSlash.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CWindSlash.h"

CSkill_WindSlash::CSkill_WindSlash()
{
}

CSkill_WindSlash::~CSkill_WindSlash()
{
}

void CSkill_WindSlash::BeginPlay()
{
	CSkill::BeginPlay();

	m_Id = "WindSlash";

	if (auto data = SKILL_DATA->GetById(m_Id))
		m_DataDamage = data->Damage;
	else
		m_DataDamage = 10.f;

	//SetIcon(L"UI\\WindSlashBasic.png", Vec2(16.f) * 2.5f);
}

void CSkill_WindSlash::Tick()
{
	CSkill::Tick();

	for (auto iter = m_WindSlash.begin(); iter != m_WindSlash.end(); )
	{
		auto wind = *iter;

		wind->Tick();

		if (wind->IsDestroy())
			iter = m_WindSlash.erase(iter);

		else
			++iter;
	}
}

void CSkill_WindSlash::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_WindSlash::Render()
{
	CSkill::Render();

	for (auto& wind : m_WindSlash)
	{
		if (!wind->IsDestroy())
			wind->Render();
	}
}

void CSkill_WindSlash::Release()
{
	CSkill::Release();
}

bool CSkill_WindSlash::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))

	// TODO : 나중에 스킬 쿨타임에 따라서 판단

	return true;

}

void CSkill_WindSlash::Activate(CActor* _Owner)
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

	auto wind = make_shared<CWindSlash>(m_Skill_Info);
	wind->BeginPlay();

	m_WindSlash.push_back(wind);
}
