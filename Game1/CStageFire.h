#pragma once

#include "CScene.h"

class CPlayer;
class CActor;
class CDummy;
class CUIComponent;

class CStageFire : public CScene
{
public:
	CStageFire();
	virtual ~CStageFire() override;

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void Key_Input();
	void Resource_Setting();

private:
	vector<shared_ptr<CActor>> m_Actors;
	vector<shared_ptr<GameObject>> m_Objects;

	Vec2 m_PlayerPos;
	Vec2 m_MapSize;

	shared_ptr<CActor> m_TestDummy;
	shared_ptr<CActor> m_ShopNPC;
	shared_ptr<CActor> blobRoller;

	shared_ptr<CUIComponent> m_PlayerUI;
	shared_ptr<ObRect> m_TileTrigger;

	bool bStartTile = false;
	bool bTrigger = false;

	Vec2 m_Section1_Pos {-1600.f, -2000.f};

private:
	template<typename T, typename... Args>
	shared_ptr<T> SpawnActor(const Vector2& pos, Args&&... args)
	{
		auto actor = make_shared<T>(forward<Args>(args)...);
		actor->SetSpawnPos(pos);
		m_Actors.push_back(actor);

		return actor; 
	}
};

