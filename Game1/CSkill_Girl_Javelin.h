#pragma once

#include "CSkill.h"

class CJavelin_Spear;

class CSkill_Girl_Javelin : public CSkill
{
public:
	CSkill_Girl_Javelin();
	virtual ~CSkill_Girl_Javelin();

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
	vector<shared_ptr<CJavelin_Spear>> m_Spear {};
};

