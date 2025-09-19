#pragma once

#include "CComponent.h"

class CSkill;

class CWeaponComponent : public CComponent
{
public:
	CWeaponComponent();
	~CWeaponComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	void Equip_Skill(ESkillType _SkillType);
	void Activate_Skill(ESkillType _SkillType);

private:
	shared_ptr<CSkill> testFireBall;

	unordered_map<ESkillType, shared_ptr<CSkill>> m_Skills;
};

