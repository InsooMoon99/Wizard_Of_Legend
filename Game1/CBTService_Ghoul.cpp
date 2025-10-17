#include "stdafx.h"
#include "CBTService_Ghoul.h"

CBTService_Ghoul::CBTService_Ghoul(float periodSec)
	: CBTService(periodSec)
{

}

CBTService_Ghoul::~CBTService_Ghoul()
{

}

void CBTService_Ghoul::OnTick(BTContext& ctx)
{
	CActor* owner = ctx.Owner;
	if (!owner) return;

	CActor* target = nullptr;
	ctx.Blackboard.SetValueAsActor(BBKey::Target, target);

	float distance = 9999.f;
	if (target)
	{
		
	}

	else
	{
		cout << "Target is Nullptr" << endl;
	}

	ctx.Blackboard.SetValueAsFloat(BBKey::ToTargetDist, distance);

}
