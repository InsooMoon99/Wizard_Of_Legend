#include "stdafx.h"
#include "CBT_Ghoul.h"

#include "CBTService_Ghoul.h"

static const char* BB_Target = "Target";
static const char* BB_ToTargetDis = "ToTargetDist";

CBT_Ghoul::CBT_Ghoul(const FGhoulParams& params)
{
	// 郴何 伎泛磐
	auto innerSelector = make_shared<CBTSelector>();
	innerSelector->AddChild(MakeAttackSeq(params));
	innerSelector->AddChild(MakeChaseSeq(params));

	// == Service 积己, 何馒 ==
	auto service = make_shared<CBTService_Ghoul>(0.2f);
	innerSelector->AddService(service);

	AddChild(innerSelector);
}

CBT_Ghoul::~CBT_Ghoul()
{

}

shared_ptr<CBTSequence> CBT_Ghoul::MakeAttackSeq(const FGhoulParams& params)
{

}

shared_ptr<CBTSequence> CBT_Ghoul::MakeChaseSeq(const FGhoulParams& params)
{

}

shared_ptr<CBTTask> CBT_Ghoul::MakeTaskFindTarget()
{

}

shared_ptr<CBTTask> CBT_Ghoul::MakeTaskIsCloseToTarget(float attackRange)
{

}

shared_ptr<CBTTask> CBT_Ghoul::MakeTaskAttack()
{

}

shared_ptr<CBTTask> CBT_Ghoul::MakeTaskChase(float attackRange, float chaseBreak)
{

}

shared_ptr<CBTTask> CBT_Ghoul::MakeTaskIdle()
{

}
