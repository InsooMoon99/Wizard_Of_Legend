#pragma once

#include "CSkill.h"

class CTarget_Orb;

class CSkill_Man_Target : public CSkill
{
public:
	CSkill_Man_Target();
	virtual ~CSkill_Man_Target();

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
	vector<shared_ptr<CTarget_Orb>> m_Orb;
};

