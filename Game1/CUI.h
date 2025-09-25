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

	void SetPos(Vec2 pos)		 { m_Pos = pos; }
	void SetRender(bool _Render) { bRender = _Render; }

	Vec2 GetPos()	 { return m_Pos; }
	bool GetRender() { return bRender; }

protected:
	Vec2	m_Pos = { -1700, -800 };
	bool	bRender = true;
};

