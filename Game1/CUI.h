#pragma once

#include "CGameObject.h"

class CUI : public CGameObject
{
public:
	CUI();
	virtual ~CUI();

public:
	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render();
	virtual void Release();

	void SetPos(Vec2 pos) { m_Pos = pos; }
	Vec2 GetPos() { return m_Pos; }

protected:
	Vec2	m_Pos = { -1700, -800 };
};

