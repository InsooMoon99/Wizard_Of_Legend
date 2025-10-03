#pragma once

#include "CMiddle_Ball.h"
#include "CSkill.h"

class CBat_Dash;

class CSkill_Middle_Ball : public CSkill
{
public:
	CSkill_Middle_Ball();
	virtual ~CSkill_Middle_Ball();

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
	vector<shared_ptr<CMiddle_Ball>> m_Ball{};
};

