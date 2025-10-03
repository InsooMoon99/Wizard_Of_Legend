#pragma once

#include "CScene.h"

class CPlayer;
class CActor;
class CDummy;
class CUIComponent;
class CMonsterBase;

class CStageFire : public CScene
{
	enum ESectionType { Section1, Section2, Section3, Section4, Section_END};

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
	vector<shared_ptr<GameObject>> m_Objects;
	unordered_map<ESectionType, vector<shared_ptr<CMonsterBase>>> m_Monsters;

	Vec2 m_PlayerPos;
	Vec2 m_MapSize;
	Vec2 m_StartPos = Vec2(521.f, -2544.f);

	shared_ptr<CMonsterBase> m_TestDummy;

	shared_ptr<CActor> m_ShopItemNPC;
	shared_ptr<CActor> m_ShopSkillNPC;

	shared_ptr<CUIComponent> m_PlayerUI;
	shared_ptr<ObRect> m_TileTrigger;

	bool bStartTilevisible = false;
	bool bTrigger = false;

	vector<Vec2> m_SectionPos;
	vector<shared_ptr<ObRect>> m_SectionCol;

	shared_ptr<ObImage> m_BossTeleport;

	shared_ptr<class CUI_Minimap> m_Minimap;

	shared_ptr<ObImage> m_Cursor;

private:
	template<typename T, typename... Args>
	shared_ptr<T> SpawnMonster(ESectionType type, const Vector2& pos, Args&&... args)
	{
		auto actor = make_shared<T>(forward<Args>(args)...);
		actor->SetSpawnPos(pos);
		m_Monsters[type].push_back(actor);

		actor->BeginPlay();

		return actor; 
	}
};

