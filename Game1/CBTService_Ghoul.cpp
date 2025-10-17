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

	CActor* target = 

}
