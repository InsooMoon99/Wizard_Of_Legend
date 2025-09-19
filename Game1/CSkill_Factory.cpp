#include "stdafx.h"
#include "CSkill_Factory.h"

#include "CSkill_FireBall.h"
#include "CSkill_WindSlash.h"

shared_ptr<CSkill> CSkill_Factory::CreateSkill(ESkillType _SkillType)
{
	switch (_SkillType)
	{
	case ESkillType::WindSlash:
		return make_shared<CSkill_WindSlash>();

	case ESkillType::FireBall:
		return make_shared<CSkill_FireBall>();

	case ESkillType::SKILL_END:
		break;
	}
}
