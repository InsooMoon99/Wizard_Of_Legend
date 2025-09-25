#pragma once

#include "CSkill.h"

class CStar;

class CSkill_Girl_Star : public CSkill
{
public:
	CSkill_Girl_Star();
	virtual ~CSkill_Girl_Star();

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
	float m_AttackTimer = 0.f;

	vector<shared_ptr<CStar>> m_Stars;

};

