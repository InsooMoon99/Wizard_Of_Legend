#pragma once

#include "CScene.h"

class CPlayer;
class CActor;
class CBoss_TwinMan;
class CBoss_TwinGirl;
class CBoss_Controller;
class CUIComponent;

class CStageBoss : public CScene
{
public:
	CStageBoss();
	virtual ~CStageBoss() override;

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
	Vec2 m_StartPos = Vec2(-1800.f, -1864.f);

	CBoss_Controller* m_BossController;
	CBoss_TwinMan*  m_Boss_Man;
	CBoss_TwinGirl* m_Boss_Girl;

	shared_ptr<CUIComponent> m_PlayerUI;
	shared_ptr<CUIComponent> m_BossUI;

	Vec2 m_ReadyPos{ -1800.f, -160.f};

	shared_ptr<ObRect> m_ReadyRect;

	bool IsFirstCheck = true;
	bool IsCameraSat = false;

	// 카메라 클램프 부드러운 진입용
	bool  m_BlendToClamp = false; // 블렌드 중인가
	float m_ClampBlendT = 0.0f;  // 0→1
	float m_ClampBlendTime = 0.4f;  // 블렌드에 걸리는 시간(초)

	bool m_ShowBossHUD = false;
};


