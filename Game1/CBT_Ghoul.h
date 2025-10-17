#pragma once

#include "CBTSelector.h"
#include "CBTSequence.h"
#include "CBTTask.h"

struct FGhoulParams
{
	float AttackRange	= 120.f;
	float ChaseBreak	= 200.f;
};

// BT_Selector를 루트로 세팅
class CBT_Ghoul : public CBTSelector
{
public:
	explicit CBT_Ghoul(const FGhoulParams& params);
	virtual ~CBT_Ghoul();

public:

private:
	// 시퀀스 세팅
	shared_ptr<CBTSequence> MakeAttackSeq(const FGhoulParams& params);
	shared_ptr<CBTSequence> MakeChaseSeq(const FGhoulParams& params);

	// 테스크 세팅
	shared_ptr<CBTTask> MakeTaskFindTarget();
	shared_ptr<CBTTask> MakeTaskIsCloseToTarget(float attackRange);
	shared_ptr<CBTTask> MakeTaskAttack();
	shared_ptr<CBTTask> MakeTaskChase(float attackRange, float chaseBreak);
	shared_ptr<CBTTask> MakeTaskIdle();

};

