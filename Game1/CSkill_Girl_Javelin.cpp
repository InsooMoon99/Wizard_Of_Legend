#include "stdafx.h"
#include "CSkill_Girl_Javelin.h"

#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CBoss_TwinGirl.h"
#include "CJavelin_Spear.h"
#include "CStateComponent.h"

CSkill_Girl_Javelin::CSkill_Girl_Javelin()
{

}

CSkill_Girl_Javelin::~CSkill_Girl_Javelin()
{

}

void CSkill_Girl_Javelin::BeginPlay()
{
	CSkill::BeginPlay();

}

void CSkill_Girl_Javelin::Tick()
{
	CSkill::Tick();

	for (auto iter = m_Spear.begin(); iter != m_Spear.end(); )
	{
		auto spear = *iter;

		spear->Tick();

		if (spear->IsDestroy())
			iter = m_Spear.erase(iter);

		else
			++iter;
	}
}

void CSkill_Girl_Javelin::LateUpdate()
{
	CSkill::LateUpdate();
}

void CSkill_Girl_Javelin::Render()
{
	CSkill::Render();

	for (auto& spear : m_Spear)
	{
		if (!spear->IsDestroy())
			spear->Render();
	}
}

void CSkill_Girl_Javelin::Release()
{
	CSkill::Release();
}

bool CSkill_Girl_Javelin::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;


	return true;

}

void CSkill_Girl_Javelin::Activate(CActor* _Owner)
{
	CSkill::Activate(_Owner);

	CBoss_TwinGirl* boss = dynamic_cast<CBoss_TwinGirl*>(_Owner);

	auto state = boss->GetControlState();
	if (!state) return;

	auto* ctrl = boss->GetController();
	//if (!ctrl->HasBallMovePos()) return;

	Vec2 dir = CHelper::DirToVec(state->Get_Dir());
	float radian = Utility::DirToRadian(dir);

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
	m_Skill_Info.Target = boss->GetPlayer();
	m_Skill_Info.Aim_Dir = dir;
	m_Skill_Info.Angle = radian;
	m_Skill_Info.TargetPos = boss->GetJavelinPos();
	m_Skill_Info.Damage = 10.f;
	m_Skill_Info.Amount = 1000.f;

	auto spear = make_shared<CJavelin_Spear>(m_Skill_Info);
	spear->BeginPlay();

	m_Spear.push_back(spear);
}
