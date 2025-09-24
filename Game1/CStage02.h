#pragma once

#include "CScene.h"

class CPlayer;
class CActor;
class CBoss_TwinMan;
class CBoss_TwinGirl;
class CBoss_Controller;
class CUIComponent;

class CStage02 : public CScene
{
public:
	CStage02();
	virtual ~CStage02() override;

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void Resource_Setting();

private:
	vector<CActor*> m_Actors;
	vector<shared_ptr<GameObject>> m_Objects;

	Vec2 m_PlayerPos;
	Vec2 m_MapSize;

	CBoss_Controller* m_BossController;
	CBoss_TwinMan*  m_Boss_Man;
	CBoss_TwinGirl* m_Boss_Girl;

	shared_ptr<CUIComponent> m_PlayerUI;
	shared_ptr<CUIComponent> m_BossUI;

};

