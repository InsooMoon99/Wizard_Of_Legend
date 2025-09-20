#pragma once

#include "CScene.h"

class CPlayer;
class CActor;
class CBoss_Light;
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
	vector<shared_ptr<ObImage>> m_Objects;

	Vec2 m_PlayerPos;
	Vec2 m_MapSize;

	CActor* m_Boss;

	CUIComponent* m_BossUI;
	CUIComponent* m_PlayerUI;

};

