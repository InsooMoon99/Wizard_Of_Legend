#pragma once

class CSkill;

class CSkill_Factory
{
public:
	CSkill_Factory()  = default;
	~CSkill_Factory() = default;

public:
	static shared_ptr<CSkill> CreateSkill(ESkillType _SkillType);

};

