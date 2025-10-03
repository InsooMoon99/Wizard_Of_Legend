#include "stdafx.h"
#include "CSkill_Factory.h"

#include "CSkill_FireBall.h"
#include "CSkill_FireDragon.h"

#include "CSkill_WindSlash.h"

#include "CSkill_Earth_Shield.h"
#include "CSkill_Girl_Dash.h"
#include "CSkill_Girl_Javelin.h"
#include "CSkill_Girl_Star.h"
#include "CSkill_Man_Bat.h"
#include "CSkill_Man_Star.h"
#include "CSkill_Man_Target.h"
#include "CSkill_FireMeteor.h"
#include "CSkill_Middle_Fire.h"
#include "CSkill_Middle_Ball.h"
#include "CSkill_Middle_SpawnBall.h"
#include "CSkill_WindDash.h"

shared_ptr<CSkill> CSkill_Factory::CreateSkill(ESkillType _SkillType)
{
	switch (_SkillType)
	{
	// Player
	case ESkillType::Fire_Ball:
		return make_shared<CSkill_FireBall>();

	case ESkillType::Fire_Dragon:
		return make_shared<CSkill_FireDragon>();

	case ESkillType::Fire_Meteor:
		return make_shared<CSkill_FireMeteor>();

	case ESkillType::Wind_Slash:
		return make_shared<CSkill_WindSlash>();

	case ESkillType::Wind_Dash:
		return make_shared<CSkill_WindDash>();

	case ESkillType::Earth_Shield:
		return make_shared<CSkill_Earth_Shield>();

	// Boss
	case ESkillType::Boss_Man_Bat:
		return make_shared<CSkill_Man_Bat>();

	case ESkillType::Boss_Girl_Dash:
		return make_shared<CSkill_Girl_Dash>();

	case ESkillType::Boss_Man_Target:
		return make_shared<CSkill_Man_Target>();

	case ESkillType::Boss_Girl_Javelin:
		return make_shared<CSkill_Girl_Javelin>();

	case ESkillType::Boss_Man_Star:
		return make_shared<CSkill_Man_Star>();

	case ESkillType::Boss_Girl_Star:
		return make_shared<CSkill_Girl_Star>();

	// Middle
	case ESkillType::Middle_Fire:
		return make_shared<CSkill_Middle_Ball>();

	case ESkillType::Middle_Ball:
		return make_shared<CSkill_Middle_Fire>();

	case ESkillType::Middle_SpawnBall:
		return make_shared<CSkill_Middle_SpawnBall>();

	case ESkillType::SKILL_END:
		break;
	}
}
