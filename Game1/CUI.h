#pragma once

class CUI
{
public:
	CUI();
	virtual ~CUI();

public:
	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render();
	virtual void Release();

	void SetPos(Vec2 pos) { m__Pos = pos; }
	Vec2 GetPos() { return m__Pos; }

	ObRect* GetRect();
	bool IsMouseInRect();

protected:
	Vec2	m__Pos = { -1700, -800 };
	Vec2    m_Size = { 150.f, 150.f };
};

