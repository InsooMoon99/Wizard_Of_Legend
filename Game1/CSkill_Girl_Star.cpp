#include "stdafx.h"
#include "CSkill_Girl_Star.h"

#include "CBossPawn.h"
#include "CStar.h"
#include "CStateComponent.h"

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

	for (auto iter = m_Stars.begin(); iter != m_Stars.end(); )
	{
		auto star = *iter;

		star->Tick();

		if (star->IsDestroy())
		{
			iter = m_Stars.erase(iter);
		}

		else
			++iter;
	}
}

void CSkill_Girl_Star::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_Girl_Star::Render()
{
	CSkill::Render();

	for (auto& star : m_Stars)
	{
		if (!star->IsDestroy())
			star->Render();
	}
}

void CSkill_Girl_Star::Release()
{
	CSkill::Release();
}

bool CSkill_Girl_Star::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;


	return true;

}

void CSkill_Girl_Star::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

	CBossPawn* boss = dynamic_cast<CBossPawn*>(_Owner);

	auto state = boss->GetControlState();
	if (!state) return;

	Vec2 dir = CHelper::DirToVec(state->Get_Dir());
	float radian = Utility::DirToRadian(dir);

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Target = boss->GetPlayer();
	m_Skill_Info.TargetPos = boss->GetPlayer()->Get_WorldPos();
	m_Skill_Info.Aim_Dir = dir;
	m_Skill_Info.Angle = radian;
	m_Skill_Info.Damage = 10.f;
	m_Skill_Info.Amount = 1000.f;

	auto star = make_shared<CStar>(m_Skill_Info);
	star->BeginPlay();

	m_Stars.push_back(star);
}
