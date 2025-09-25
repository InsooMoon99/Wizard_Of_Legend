#pragma once

#include "CSkill.h"

class CStar;

class CSkill_Man_Star : public CSkill
{
public:
	CSkill_Man_Star();
	virtual ~CSkill_Man_Star();

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
	
};

