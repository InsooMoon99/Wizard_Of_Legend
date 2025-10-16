#pragma once

#include "CBTNode.h"

class CBTDecorator : public CBTNode
{
public:
	CBTDecorator() {}
	virtual ~CBTDecorator() {}

public:
	// 데코레이터는 단 1개의 자식 노드만 소유
	void SetChild(shared_ptr<CBTNode> _Node)
	{
		m_Child = _Node;
	}

protected:
	shared_ptr<CBTNode> m_Child;

};

class CBTInverter : public CBTDecorator
{
public:
	CBTInverter() {}
	virtual ~CBTInverter() {}

public:
	BTResult Tick(BTContext& ctx) override;

};

