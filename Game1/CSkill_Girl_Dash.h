#pragma once

#include "CSkill.h"

class CBat_Dash;

class CSkill_Girl_Dash : public CSkill
{
public:
	CSkill_Girl_Dash();
	virtual ~CSkill_Girl_Dash();

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
	vector<shared_ptr<CBat_Dash>> m_Dash{};
};

