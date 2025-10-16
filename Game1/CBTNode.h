#pragma once

enum class BTResult
{
	Success, Failure, Running
};

struct BTContext
{
	CActor* Owner;

	map<string, void*> Blackboard;
};


class CBTNode
{
public:
	CBTNode() = default;
	virtual ~CBTNode() = default;

public:
	virtual BTResult Tick(BTContext& ctx) abstract;
};