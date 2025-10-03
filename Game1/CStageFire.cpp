#include "stdafx.h"
#include "CStageFire.h"
#include "CActor.h"
#include "CBlobRoller.h"
#include "CBoss_TwinMan.h"
#include "CDummy.h"
#include "CGhoulLarge.h"
#include "CKnight.h"
#include "CMonsterBase.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CShopItemNPC.h"
#include "CStateComponent.h"
#include "CUIComponent.h"
#include "CGolem.h"
#include "CItem_Gold.h"
#include "CItem_SkillMeteor.h"
#include "CMiddleBoss.h"
#include "CShopSkillNPC.h"
#include "CUI_Minimap.h"

CStageFire::CStageFire()
{
	m_MapSize = Vec2(2304.f, 2560.f) * 3.f;
	m_PlayerPos = Vec2(516.f, -2593.f);

	m_SectionPos.push_back({-1600.f,-2000.f});
	m_SectionPos.push_back({7.f, -1011.f});
	m_SectionPos.push_back({-90.f, 380.f});
	m_SectionPos.push_back({1094.f, 1175.f});

	Resource_Setting();
	m_Actors.push_back(m_Player);

	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player.get());

	bStartTilevisible = false;

	//m_Cursor = RESOURCE->CreateImage(L"UI\\MouseCursor0.png", Vec2(32.f, 32.f) * 3.f, true);
	//m_Cursor->space = SPACE::SCREEN;
}

CStageFire::~CStageFire()
{
	Release();
}

void CStageFire::Init()
{
	CScene::Init();

	SOUND->Play("Tutorial");
	SOUND->SetVolume("Tutorial", 0.05f);

	PARTICLE->Spawn_ParticleDir(L"StartEffect", Vec2(m_PlayerPos.x, m_PlayerPos.y + 180.f), Vec2(1.4f, 1.f));
	SOUND->StopPlay("Teleport", 0.2f);

	for (auto& actor : m_Actors)
		actor->BeginPlay();

	for (auto& [sec, vec] : m_Monsters)
		for (auto& m : vec)
			m->BeginPlay();

	bStartTilevisible = true;
	  
	if (bStartTilevisible)
	{
		TILE->Load_Data(L"../Data/FireTile_First.dat");
		//TILE->Load_Data(L"../Data/FireTile_Second.dat");
		bStartTilevisible = false;
	}

	auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	m_Minimap = make_shared<CUI_Minimap>(player);

	auto mapImg = m_Map.find(ESceneType::Stage01)->second;
	Vec2 mapCenter = mapImg->GetWorldPos(); // (0, 0)
	Vec2 mapHalf = mapImg->scale * 0.5f;

	Vec2 worldMin = mapCenter - mapHalf; // 예: (-W/2, -H/2)
	Vec2 worldMax = mapCenter + mapHalf; // 예: (W/2, H/2)

	m_Minimap->SetWorldBounds(worldMin, worldMax);

	m_Minimap->BeginPlay();
}

void CStageFire::Update()
{
	CScene::Update();

	//ShowCursor(false);

	Key_Input();

	m_TileTrigger->Update();

	for (auto& actor : m_Actors)
	{
		actor->Tick();
		RENDER->AddRenderObject(actor.get());
	}

	if (INPUT->KeyDown('2'))
	{
		auto middleBoss = SpawnMonster<CMiddleBoss>(Section4, Vec2(1986, 2654));
		middleBoss->SetTarget(m_Player);
	}

	RemoveDeadMonsters();

	if (m_Minimap)
		m_Minimap->Tick();

	for (auto& [sec, vec] : m_Monsters)
		for (auto& m : vec) { m->Tick();
			RENDER->AddRenderObject(m.get()); }

	for (auto& section : m_SectionCol)
		section->Update();

	for (const auto& hud : m_PlayerUI->GetHUD())
		RENDER->AddRenderObject(hud.get());

	for (auto& map : m_Map)
		map.second->Update();
	
	for (auto& object : m_Objects)
		object->Update();

	if (m_Minimap)
		RENDER->AddRenderObject(m_Minimap.get());

	//m_Cursor->SetWorldPos(INPUT->GetMouseWorldPos());
	//m_Cursor->Update();

}

void CStageFire::LateUpdate()
{
	CScene::LateUpdate();

	//플레이어 가두기 이건 딱히 필요 없을듯. 타일맵으로 충돌처리
	//player->Get_PlayerCollision()->SetWorldPosX(Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().x, -3800.0f * 1.5f / 2.0f, 3840.0f * 1.5f / 2.0f));
	//캠
	//CAM->position.x = Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().x, -m_MapSize.x + app.GetHalfWidth(), m_MapSize.x - app.GetHalfWidth());
	//CAM->position.y = Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().y + 500.0f, m_MapSize.y, 950.0f - app.GetHalfHeight());

	auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	CAM->position = player->Get_Collision()->GetWorldPos();

	if(m_TileTrigger->Intersect(m_Player->Get_Collision().get()) && !bTrigger)
	{
		TILE->Load_Data(L"../Data/FireTile_Second.dat");

		bTrigger = true;
	}

	for (int i = 0; i < Section_END; i++)
	{
		if (m_SectionCol[i]->Intersect(m_Player->Get_Collision().get()))
		{
			for (auto& [sec, vec] : m_Monsters)
			{
				if (sec == i)
				{
					for (auto& monster : vec)
					{
						if (monster && monster->GetTarget() == nullptr)
							monster->SetTarget(m_Player);
					}
				}
			}
		}
	}

	if (m_BossTeleport->Intersect(m_Player->Get_Collision().get()))
	{
		if (INPUT->KeyDown('F'))
		{
			PARTICLE->Spawn_Particle(L"StartEffect", Vec2(m_BossTeleport->GetWorldPos().x, m_BossTeleport->GetWorldPos().y + 300.f), Vec2(1.5f));
			SOUND->StopPlay("Teleport", 0.2f);
		}
	}
	

	for (auto& actor : m_Actors)
		actor->LateUpdate();

	for (auto& [sec, vec] : m_Monsters)
		for (auto& m : vec) m->LateUpdate();
}

void CStageFire::Render()
{
	CScene::Render();

	for (auto& map : m_Map)
		map.second->Render();

	TILE->Render();
	m_TileTrigger->Render();

	for (auto& object : m_Objects)
		object->Render();

	for (auto& section : m_SectionCol)
		section->Render();

	PARTICLE->Render();
	//m_Cursor->Render();

}

void CStageFire::Release()
{
	CScene::Release();

	//for (auto& actor : m_Actors)
	//	SafeDelete(actor);

	m_Map.clear();

	SOUND->DeleteSound("Tutorial");
}

void CStageFire::Key_Input()
{
	// 타일맵 visible
	if (INPUT->KeyDown(VK_F1))
		TILE->ToggleTileImagesVisible();

	// 타겟 디버깅
	if (INPUT->KeyDown('1'))
	{
		for (auto& actor : m_Actors)
		{
			CMonsterBase* monster = dynamic_cast<CMonsterBase*>(actor.get());
			if (monster)
				monster->SetTarget(m_Player);
		}
	}
}

void CStageFire::Resource_Setting()
{
	{
		m_TileTrigger = make_shared<ObRect>();
		m_TileTrigger->scale = Vec2(200.f, 200.f);
		m_TileTrigger->SetWorldPos(Vec2(6.f, -1366.f));
		m_TileTrigger->visible = false;
		m_TileTrigger->isFilled = false;
		m_TileTrigger->collider = COLLIDER::RECT;
	}

	// Map
	shared_ptr<ObImage> mapShadow = RESOURCE->CreateImage(L"Edit\\SHADOW.bmp", m_MapSize * 1.f, true);
	mapShadow->SetWorldPos(Vec2(0, 0));
	mapShadow->color = Color(0.0f, 0.0f, 0.0f, 1.0f);
	m_Map.insert({ ESceneType::Shadow, mapShadow });

	shared_ptr<ObImage> map = RESOURCE->CreateImage(L"Edit\\FireTile.png", m_MapSize, true);
	map->SetWorldPos(Vec2(0, 0));
	map->SetLocalPos(Vec2(-185, 46));
	m_Map.insert({ ESceneType::Stage01, map });

	{
		Vec2 telpoSize = Vec2(311.f, 298.f);
		shared_ptr<ObImage> telport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", telpoSize, true);
		telport->SetWorldPos(Vec2(516.f, -2593.f));
		m_Objects.push_back(telport);
	}
	{
		Vec2 telpoSize = Vec2(311.f, 298.f);
		m_BossTeleport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", telpoSize, true);
		m_BossTeleport->SetWorldPos(Vec2(1966.f, 3006.f));
		m_BossTeleport->collider = COLLIDER::RECT;
		m_Objects.push_back(m_BossTeleport);
	}

	m_Player = make_shared<CPlayer>();

	//m_Player->SetSpawnPos(Vec2(m_SectionPos[3].x + 170.f, m_SectionPos[3].y - 100.f));
	//m_Player->SetSpawnPos(Vec2(2225.f, 849.f));
	m_Player->SetSpawnPos(m_StartPos);

	m_TestDummy = SpawnMonster<CDummy>(Section1, m_PlayerPos + Vec2(200.f, 100.f));
	shared_ptr<CDummy> testDummy2 = SpawnMonster<CDummy>(Section1, m_PlayerPos + Vec2(200.f, 20.f));
	shared_ptr<CDummy> testDummy3 = SpawnMonster<CDummy>(Section1, Vec2(2450.f, -2500.f));

	m_ShopItemNPC	= make_shared<CShopItemNPC>();
	m_ShopSkillNPC	= make_shared<CShopSkillNPC>();

	m_ShopItemNPC->SetSpawnPos(Vec2(2120., -2237.f));
	m_ShopSkillNPC->SetSpawnPos(Vec2(2384.f, 1071.f));

	m_Actors.push_back(m_ShopItemNPC);
	m_Actors.push_back(m_ShopSkillNPC);

	auto ghoul1 = SpawnMonster<CGhoulLarge>(Section1, Vec2(m_SectionPos[0].x + 100.f, m_SectionPos[0].y));
	auto ghoul2 = SpawnMonster<CGhoulLarge>(Section1, Vec2(m_SectionPos[0].x - 100.f, m_SectionPos[0].y));

	auto knight1 = SpawnMonster<CKnight>(Section1, Vec2(m_SectionPos[0].x - 100, m_SectionPos[0].y + 200.f));
	auto knight2 = SpawnMonster<CKnight>(Section1, Vec2(m_SectionPos[0].x - 100, m_SectionPos[0].y - 150.f));
	auto knight3 = SpawnMonster<CKnight>(Section1, Vec2(m_SectionPos[0].x + 100, m_SectionPos[0].y + 200.f));
	auto knight4 = SpawnMonster<CKnight>(Section1, Vec2(m_SectionPos[0].x + 100, m_SectionPos[0].y - 150.f));

	auto blob1 = SpawnMonster<CBlobRoller>(Section2, Vec2(m_SectionPos[1].x + 40.f, m_SectionPos[1].y + 150));
	auto blob2 = SpawnMonster<CBlobRoller>(Section2, Vec2(m_SectionPos[1].x - 40.f, m_SectionPos[1].y + 120));
	auto blob3 = SpawnMonster<CBlobRoller>(Section2, Vec2(m_SectionPos[1].x + 20.f, m_SectionPos[1].y + 10));

	//auto knight1 = SpawnMonster<CKnight>(Section2, Vec2(m_SectionPos[1].x, m_SectionPos[1].y + 200.f));
	auto golem1 = SpawnMonster<CGolem>(Section3, Vec2(m_SectionPos[2].x, m_SectionPos[2].y + 300.f));
	auto golem2 = SpawnMonster<CGolem>(Section3, Vec2(m_SectionPos[2].x + 219.f, m_SectionPos[2].y + 350.f));

	{
		auto blob1 = SpawnMonster<CBlobRoller>(Section3, Vec2(m_SectionPos[2].x + 240.f, m_SectionPos[2].y + 250));
		auto blob2 = SpawnMonster<CBlobRoller>(Section3, Vec2(m_SectionPos[2].x - 40.f, m_SectionPos[2].y + 120));
		auto blob3 = SpawnMonster<CBlobRoller>(Section3, Vec2(m_SectionPos[2].x + 120.f, m_SectionPos[2].y + 100));
	}

	{
		auto ghoul1 = SpawnMonster<CGhoulLarge>(Section4, Vec2(800, 1600));
		auto ghoul2 = SpawnMonster<CGhoulLarge>(Section4, Vec2(500, 1550));

		auto knight1 = SpawnMonster<CKnight>(Section4, Vec2(400, 1600));
		auto knight2 = SpawnMonster<CKnight>(Section4, Vec2(900, 1500));
	}

	//auto middleBoss = SpawnMonster<CMiddleBoss>(Section4, Vec2(m_SectionPos[3].x, m_SectionPos[3].y + 300.f));

	{
		auto section1 = make_shared<ObRect>();
		section1->scale = Vec2(30.f, 160.f);
		section1->visible = false;
		section1->isFilled = false;
		section1->SetWorldPos(Vec2(m_SectionPos[0].x + 700.f, m_SectionPos[0].y + 10.f));
		m_SectionCol.push_back(section1);

		auto section2 = make_shared<ObRect>();
		section2->scale = Vec2(300.f, 60.f);
		section2->visible = false;
		section2->isFilled = false;
		section2->pivot = OFFSET_LT;
		section2->SetWorldPos(Vec2(m_SectionPos[1].x - 150.f, m_SectionPos[1].y + 10.f));
		m_SectionCol.push_back(section2);

		auto section3 = make_shared<ObRect>();
		section3->scale = Vec2(600.f, 60.f);
		section3->visible = false;
		section3->isFilled = false;
		section3->pivot = OFFSET_LT;
		section3->SetWorldPos(Vec2(m_SectionPos[2].x, m_SectionPos[2].y + 10.f));
		m_SectionCol.push_back(section3);

		auto section4 = make_shared<ObRect>();
		section4->scale = Vec2(400.f, 200.f);
		section4->visible = false;
		section4->isFilled = false;
		section4->pivot = OFFSET_LT;
		section4->SetWorldPos(Vec2(m_SectionPos[3].x, m_SectionPos[3].y + 100.f));
		m_SectionCol.push_back(section4);
	}

}

void CStageFire::RemoveDeadMonsters()
{
	for (auto& [sec, vec] : m_Monsters)
	{
		vec.erase(remove_if(vec.begin(), vec.end(),
			[](const shared_ptr<CMonsterBase>& m)
			{
				return !m || m->IsDestroy();
			}),
			vec.end());
	}
}


