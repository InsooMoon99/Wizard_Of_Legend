#pragma once

#include "CSkill.h"

//class CWindSlash;

class CSkill_WindDash : public CSkill
{
public:
	CSkill_WindDash();
	~CSkill_WindDash() override;

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
	//vector<shared_ptr<CWindSlash>> m_WindSlash;

};

