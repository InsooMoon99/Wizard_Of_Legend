#pragma once

#include "CSkill.h"

class CEarth_Shield;

class CSkill_Earth_Shield : public CSkill
{
public:
	CSkill_Earth_Shield();
	~CSkill_Earth_Shield() override;

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
	vector<shared_ptr<CEarth_Shield>> m_Armor;

};

