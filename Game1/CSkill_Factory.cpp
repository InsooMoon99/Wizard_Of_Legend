#include "stdafx.h"
#include "CSkill_Factory.h"

#include "CSkill_FireBall.h"
#include "CSkill_FireDragon.h"

#include "CSkill_WindSlash.h"

#include "CSkill_Earth_Shield.h"

shared_ptr<CSkill> CSkill_Factory::CreateSkill(ESkillType _SkillType)
{
	switch (_SkillType)
	{
	case ESkillType::Fire_Ball:
		return make_shared<CSkill_FireBall>();

	case ESkillType::Fire_Dragon:
		return make_shared<CSkill_FireDragon>();

	case ESkillType::Wind_Slash:
		return make_shared<CSkill_WindSlash>();

	case ESkillType::Earth_Shield:
		return make_shared<CSkill_Earth_Shield>();

	case ESkillType::SKILL_END:
		break;
	}
}
