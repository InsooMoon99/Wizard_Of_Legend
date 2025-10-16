#pragma once

#include "CBTNode.h"

class CBTTask : public CBTNode
{
public:
	CBTTask();
	virtual ~CBTTask();

public:
	void SetExecute(const function<BTResult(BTContext)>& fn) { m_Exec = fn; }

	BTResult Tick(BTContext& ctx) override
	{
		return m_Exec ? m_Exec(ctx) : BTResult::Failure;
	}

private:
	function<BTResult(BTContext)> m_Exec;
};

