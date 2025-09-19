#include "stdafx.h"
#include "CSkill_Factory.h"

#include "CSkill_FireBall.h"
#include "CSkill_Gaia_Armor.h"
#include "CSkill_WindSlash.h"

shared_ptr<CSkill> CSkill_Factory::CreateSkill(ESkillType _SkillType)
{
	switch (_SkillType)
	{
	case ESkillType::Wind_Slash:
		return make_shared<CSkill_WindSlash>();

	case ESkillType::Fire_Ball:
		return make_shared<CSkill_FireBall>();

	case ESkillType::Gaia_Armor:
		return make_shared<CSkill_Gaia_Armor>();

	case ESkillType::SKILL_END:
		break;
	}
}
