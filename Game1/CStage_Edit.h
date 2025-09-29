#pragma once

#include "CScene.h"

class CStage_Edit : public CScene
{
public:
	CStage_Edit();
	virtual ~CStage_Edit();

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void Key_Input();

private:
	vector<shared_ptr<CActor>> m_Actors;

	Vec2 m_PlayerPos;
	Vec2 m_MapSize;

	bool m_LBtnPainting = false;
	Int2 m_LastPaintCell = { INT_MIN, INT_MIN };
};

