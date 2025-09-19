#pragma once

#include "CSkill.h"

class CGaia_Armor;

class CSkill_Gaia_Armor : public CSkill
{
public:
	CSkill_Gaia_Armor();
	~CSkill_Gaia_Armor() override;

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
	vector<shared_ptr<CGaia_Armor>> m_Armor;

};

