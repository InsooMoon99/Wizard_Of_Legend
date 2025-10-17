#pragma once

#include "CBTNode.h"
#include "CBTService.h"

class CBTService;

class CBTComposite : public CBTNode
{
public:
	CBTComposite() {}
	virtual ~CBTComposite() {}

public:
	void AddChild(shared_ptr<CBTNode> node)
	{
		m_Children.push_back(node);
	}

	void AddService(const shared_ptr<CBTService>& service) { m_Services.push_back(svc); }
	void ClearServices() { m_Services.clear(); }

protected:
	vector<shared_ptr<CBTNode>> m_Children;
	int32 m_RunningIdx = 0;

	vector<shared_ptr<CBTService>> m_Services;
	void TickServices(BTContext& ctx, float delta);


};

