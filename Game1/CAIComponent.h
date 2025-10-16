#pragma once

#include "CBTNode.h"
#include "CComponent.h"

class CBTNode;

class CAIComponent : public CComponent
{
public:
	CAIComponent();
	CAIComponent(CActor* _Owner) { m_Owner = _Owner; }
	~CAIComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	void SetRootNode(shared_ptr<CBTNode> _Root) { m_RootNode = _Root; }

private:
	CActor* m_Owner;
	shared_ptr<CBTNode> m_RootNode;
	BTContext m_Context;
};

