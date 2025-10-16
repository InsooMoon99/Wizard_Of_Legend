#include "stdafx.h"
#include "CBT_Ghoul.h"

static const char* BB_Target = "Target";

CBT_Ghoul::CBT_Ghoul(const FGhoulParams& params)
{
	// 郴何 伎泛磐
	auto innerSelector = make_shared<CBTSelector>();
	innerSelector->AddChild(MakeAttackSeq(params));
	innerSelector->AddChild(MakeChaseSeq(params));

	// == Service 积己, 何馒 ==
	auto service = make_shared<CBTService>(0.2f);
	service->SetOnTick([](BTContext& ctx)
		{
			CActor* ghoul = ctx.Owner;
		//CActor* target = Find
		});

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
