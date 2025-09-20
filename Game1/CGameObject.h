#pragma once

#include "CEnums.h"

class CGameObject abstract
{
public:
	CGameObject(ERenderLayer _RenderLayer);
	virtual ~CGameObject();

public:
	virtual void BeginPlay() {};
	virtual void Tick() {};
	virtual void LateUpdate() {};
	virtual void Render() {};
	virtual void Release() {};

public:
	ERenderLayer GetRenderLayer() const { return m_RenderLayer; }

protected:
	ERenderLayer m_RenderLayer = ERenderLayer::Render_GameObject;
};

