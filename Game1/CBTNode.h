#pragma once
#include "CBlackboard.h"

enum class BTResult
{
	Success, Failure, Running
};

struct BTContext
{
	CActor* Owner = nullptr;
	CBlackboard Blackboard;
};


class CBTNode
{
public:
	CBTNode() = default;
	virtual ~CBTNode() = default;

public:
	virtual BTResult Tick(BTContext& ctx) abstract;
};