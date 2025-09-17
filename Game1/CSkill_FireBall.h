#pragma once

#include "CFireBall.h"
#include "CSkill.h"

class CActor;

class CSkill_FireBall : public CSkill
{
public:
	CSkill_FireBall();
	virtual ~CSkill_FireBall();

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;

public:
	SkillInfo& Get_Skill_Info() { return m_Skill_Info; }

	bool CanActivate(const SkillInfo& _Skill) override;
	void Activate(CActor* _Owner) override;

private:
	SkillInfo m_Skill_Info;
	vector<shared_ptr<CFireBall>> m_FireBall;

};

