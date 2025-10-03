#include "stdafx.h"
#include "CSkill_Man_Bat.h"
#include "CBat_Ball.h"
#include "CBossPawn.h"
#include "CCharacter.h"
#include "CStateComponent.h"

CSkill_Man_Bat::CSkill_Man_Bat()
{

}

CSkill_Man_Bat::~CSkill_Man_Bat()
{

}

void CSkill_Man_Bat::BeginPlay()
{
	CSkill::BeginPlay();

}

void CSkill_Man_Bat::Tick()
{
	CSkill::Tick();

	for (auto iter = m_BatBall.begin(); iter != m_BatBall.end(); )
	{
		auto batball = *iter;

		batball->Tick();

		if (batball->IsDestroy())
			iter = m_BatBall.erase(iter);

		else
			++iter;
	}
}

void CSkill_Man_Bat::LateUpdate()
{
	CSkill::LateUpdate();

}

void CSkill_Man_Bat::Render()
{
	CSkill::Render();

	for (auto& batBall : m_BatBall)
	{
		if (!batBall->IsDestroy())
			batBall->Render();
	}

}

void CSkill_Man_Bat::Release()
{
	CSkill::Release();

}

bool CSkill_Man_Bat::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;

	// TODO : 보스는 굳이 이거 판단해야 하나?

	return true;

}

void CSkill_Man_Bat::Activate(CActor* _Owner)
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

	auto batBall = make_shared<CBat_Ball>(m_Skill_Info);
	batBall->BeginPlay();

	m_BatBall.push_back(batBall);
}
