#pragma once

#include "CDelegate.h"
#include "CGameObject.h"

class CUI : public CGameObject
{
public:
	CUI();
	virtual ~CUI();

public:
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render() override;
	virtual void Release() override;

	void SetPos(Vec2 pos)		 { m_Pos = pos; }
	virtual void SetRender(bool _Render) { bRender = _Render; }

	Vec2 GetPos()	 { return m_Pos; }
	bool GetRender() { return bRender; }

	CDelegate<bool> FOnRendering;

protected:
	Vec2	m_Pos = { -1700, -800 };
	bool	bRender = true;
};

