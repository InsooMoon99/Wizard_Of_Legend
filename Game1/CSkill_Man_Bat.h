#pragma once

#include "CSkill.h"

class CBat_Ball;

class CSkill_Man_Bat : public CSkill
{
public:
	CSkill_Man_Bat();
	virtual ~CSkill_Man_Bat();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CanActivate(const FSkillInfo& _Info) override;
	void Activate(CActor* _Owner) override;

private:
	vector<shared_ptr<CBat_Ball>> m_BatBall;
};

