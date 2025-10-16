#include "stdafx.h"
#include "CMonster_Factory.h"

#include "CMonster_Melee.h"

unique_ptr<IMonsterAction> CMonster_Factory::CreateMelee(CMonsterBase* _Owner, const FMeleeParams& params)
{
	auto action = make_unique<CMonster_Melee>();
	action->Settings(params);

	return action;
}
