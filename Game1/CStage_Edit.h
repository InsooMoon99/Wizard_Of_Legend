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
	vector<CActor*> m_Actors;

	Vec2 m_PlayerPos;
};

