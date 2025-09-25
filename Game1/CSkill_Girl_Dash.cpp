#include "stdafx.h"
#include "CSkill_Girl_Dash.h"
#include "CBat_Dash.h"
#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CCharacter.h"
#include "CStateComponent.h"

CSkill_Girl_Dash::CSkill_Girl_Dash()
{

}

CSkill_Girl_Dash::~CSkill_Girl_Dash()
{

}

void CSkill_Girl_Dash::BeginPlay()
{
	CSkill::BeginPlay();

}

void CSkill_Girl_Dash::Tick()
{
	CSkill::Tick();

	for (auto iter = m_Dash.begin(); iter != m_Dash.end(); )
	{
		auto dash = *iter;

		dash->Tick();

		if (dash->IsDestroy())
			iter = m_Dash.erase(iter);

		else
			++iter;
	}

}

void CSkill_Girl_Dash::LateUpdate()
{
	CSkill::LateUpdate();

}

void CSkill_Girl_Dash::Render()
{
	CSkill::Render();

	for (auto& batBall : m_Dash)
	{
		if (!batBall->IsDestroy())
			batBall->Render();
	}
}

void CSkill_Girl_Dash::Release()
{
	CSkill::Release();

}

bool CSkill_Girl_Dash::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;

	// TODO : 보스는 굳이 이거 판단해야 하나?

	return true;

}

void CSkill_Girl_Dash::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

	CBossPawn* boss = dynamic_cast<CBossPawn*>(_Owner);

	auto state = boss->GetControlState();
	if (!state) return;

	auto* ctrl = boss->GetController();
	//if (!ctrl->HasBallMovePos()) return;

	Vec2 dir = CHelper::DirToVec(state->Get_Dir());

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Target = boss->GetPlayer();
	m_Skill_Info.TargetPos = ctrl->GetBallMovePos();
	m_Skill_Info.Damage = 10.f;
	m_Skill_Info.Amount = 1000.f;

 	auto dash = make_shared<CBat_Dash>(m_Skill_Info);
	dash->BeginPlay();

	m_Dash.push_back(dash);
}
