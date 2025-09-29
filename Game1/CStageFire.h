#pragma once

#include "CScene.h"

class CPlayer;
class CActor;
class CDummy;
class CUIComponent;
class CMonsterBase;

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
	void RemoveDeadMonsters();

private:
	vector<shared_ptr<CActor>> m_Actors;
	vector<shared_ptr<CMonsterBase>> m_Monsters;
	vector<shared_ptr<GameObject>> m_Objects;

	Vec2 m_PlayerPos;
	Vec2 m_MapSize;

	shared_ptr<CMonsterBase> m_TestDummy;

	shared_ptr<CActor> m_ShopNPC;

	shared_ptr<CUIComponent> m_PlayerUI;
	shared_ptr<ObRect> m_TileTrigger;

	bool bStartTile = false;
	bool bTrigger = false;

	Vec2 m_Section1_Pos {-1600.f, -2000.f};
	shared_ptr<ObRect> m_Section1Col;

private:
	template<typename T, typename... Args>
	shared_ptr<T> SpawnMonster(const Vector2& pos, Args&&... args)
	{
		auto actor = make_shared<T>(forward<Args>(args)...);
		actor->SetSpawnPos(pos);
		m_Monsters.push_back(actor);

		return actor; 
	}
};

