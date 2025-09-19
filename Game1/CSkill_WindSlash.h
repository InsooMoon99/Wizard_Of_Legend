#pragma once

#include "CSkill.h"

class CWindSlash;

class CSkill_WindSlash : public CSkill
{
public:
	CSkill_WindSlash();
	~CSkill_WindSlash() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CanActivate(const SkillInfo& _Info) override;
	void Activate(CActor* _Owner) override;

private:
	vector<shared_ptr<CWindSlash>> m_WindSlash;

};

