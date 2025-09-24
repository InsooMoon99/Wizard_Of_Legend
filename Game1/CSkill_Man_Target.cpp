#include "stdafx.h"
#include "CSkill_Man_Target.h"

#include "CBossPawn.h"
#include "CStateComponent.h"
#include "CTarget_Orb.h"

CSkill_Man_Target::CSkill_Man_Target()
{

}

CSkill_Man_Target::~CSkill_Man_Target()
{

}

void CSkill_Man_Target::BeginPlay()
{
	CSkill::BeginPlay();

}

void CSkill_Man_Target::Tick()
{
	CSkill::Tick();

	for (auto iter = m_Orb.begin(); iter != m_Orb.end(); )
	{
		auto orb = *iter;

		orb->Tick();

		if (orb->IsDestroy())
			iter = m_Orb.erase(iter);

		else
			++iter;
	}
}

void CSkill_Man_Target::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_Man_Target::Render()
{
	CSkill::Render();

	for (auto& orb : m_Orb)
	{
		if (!orb->IsDestroy())
			orb->Render();
	}
}

void CSkill_Man_Target::Release()
{
	CSkill::Release();
}

bool CSkill_Man_Target::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;

	// TODO : 보스는 굳이 이거 판단해야 하나?

	return true;
}

void CSkill_Man_Target::Activate(CActor* _Owner)
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
	m_Skill_Info.Aim_Dir = dir;
	m_Skill_Info.Angle = radian;
	m_Skill_Info.Damage = 10.f;
	m_Skill_Info.Amount = 1000.f;

	auto orb = make_shared<CTarget_Orb>(m_Skill_Info);
	orb->BeginPlay();

	m_Orb.push_back(orb);
}
