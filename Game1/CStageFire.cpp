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
#include "CShopSkillNPC.h"

CStageFire::CStageFire()
{
	m_MapSize = Vec2(2304.f, 2560.f) * 3.f;
	m_PlayerPos = Vec2(516.f, -2593.f);

	m_SectionPos.push_back({-1600.f,-2000.f});
	m_SectionPos.push_back({7.f, -1011.f});
	m_SectionPos.push_back({-90.f, 380.f});

	Resource_Setting();
	m_Actors.push_back(m_Player);

	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player.get());

	bStartTilevisible = false;
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

	PARTICLE->Spawn_ParticleDir(L"StartEffect", Vec2(m_PlayerPos.x, m_PlayerPos.y + 180.f), Vec2(1.2f, 1.f));

	for (auto& actor : m_Actors)
		actor->BeginPlay();

	for (auto& [sec, vec] : m_Monsters)
		for (auto& m : vec)
			m->BeginPlay();

	bStartTilevisible = true;
	  
	if (bStartTilevisible)
	{
		TILE->Load_Data(L"../Data/FireTile_First.dat");
		bStartTilevisible = false;
	}

}

void CStageFire::Update()
{
	CScene::Update();

	Key_Input();

	m_TileTrigger->Update();

	for (auto& actor : m_Actors)
	{
		actor->Tick();
		RENDER->AddRenderObject(actor.get());
	}

	RemoveDeadMonsters();

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

	shared_ptr<ObImage> map = RESOURCE->CreateImage(L"Edit\\Fire_End.png", m_MapSize, true);
	map->SetWorldPos(Vec2(0, 0));
	map->SetLocalPos(Vec2(-185, 46));
	m_Map.insert({ ESceneType::Stage01, map });

	Vec2 telpoSize = Vec2(311.f, 298.f);
	shared_ptr<ObImage> telport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", telpoSize, true);
	telport->SetWorldPos(Vec2(516.f, -2593.f));
	m_Objects.push_back(telport);
	m_Player		= make_shared<CPlayer>();

	m_Player->SetSpawnPos(Vec2(m_SectionPos[2].x, m_SectionPos[2].y - 200.f));
	m_TestDummy = SpawnMonster<CDummy>(Section1, m_PlayerPos + Vec2(150.f, 50.f));

	m_ShopItemNPC	= make_shared<CShopItemNPC>();
	m_ShopSkillNPC	= make_shared<CShopSkillNPC>();

	m_ShopItemNPC->SetSpawnPos(Vec2(m_PlayerPos.x, m_PlayerPos.y - 50.f));
	m_ShopSkillNPC->SetSpawnPos(Vec2(m_SectionPos[2].x, m_SectionPos[2].y - 230.f));

	m_Actors.push_back(m_ShopItemNPC);
	m_Actors.push_back(m_ShopSkillNPC);

	auto blob1 = SpawnMonster<CBlobRoller>(Section1, Vec2(m_SectionPos[0].x + 100.f, m_SectionPos[0].y));
	auto blob2 = SpawnMonster<CBlobRoller>(Section1, Vec2(m_SectionPos[0].x - 40.f, m_SectionPos[0].y + 20));
	auto blob3 = SpawnMonster<CBlobRoller>(Section1, Vec2(m_SectionPos[0].x + 20.f, m_SectionPos[0].y + 10));

	auto ghoul = SpawnMonster<CGhoulLarge>(Section1, Vec2(m_SectionPos[0].x + 100.f, m_SectionPos[0].y));

	auto knight1 = SpawnMonster<CKnight>(Section2, Vec2(m_SectionPos[1].x, m_SectionPos[1].y + 200.f));
	auto golem = SpawnMonster<CGolem>(Section3, Vec2(m_SectionPos[2].x, m_SectionPos[2].y + 200.f));
	auto flamezoner = SpawnMonster<CGolem>(Section3, Vec2(m_SectionPos[2].x + 99.f, m_SectionPos[2].y + 150.f));

	auto section1 = make_shared<ObRect>();
	section1->scale = Vec2(30.f, 160.f);
	section1->visible = true;
	section1->isFilled = false;
	section1->SetWorldPos(Vec2(m_SectionPos[0].x + 700.f, m_SectionPos[0].y + 10.f));
	m_SectionCol.push_back(section1);

	auto section2 = make_shared<ObRect>();
	section2->scale = Vec2(300.f, 60.f);
	section2->visible = true;
	section2->isFilled = false;
	section2->pivot = OFFSET_LT;
	section2->SetWorldPos(Vec2(m_SectionPos[1].x - 150.f, m_SectionPos[1].y + 10.f));
	m_SectionCol.push_back(section2);

	auto section3 = make_shared<ObRect>();
	section3->scale = Vec2(600.f, 60.f);
	section3->visible = true;
	section3->isFilled = false;
	section3->pivot = OFFSET_LT;
	section3->SetWorldPos(Vec2(m_SectionPos[2].x, m_SectionPos[2].y + 10.f));
	m_SectionCol.push_back(section3);

	// TODO : section 4 추가 예정
	m_SectionCol.push_back(section2);

	// 임시 골드
	{
		auto gold = make_shared<CItem_Gold>();
		gold->SetSpawnPos(Vec2(0.f));

		auto gold1 = make_shared<CItem_Gold>();
		gold1->SetSpawnPos(Vec2(10.f));

		auto gold2 = make_shared<CItem_Gold>();
		gold2->SetSpawnPos(Vec2(15.f));

		auto gold3 = make_shared<CItem_Gold>();
		gold3->SetSpawnPos(Vec2(20.f));

		auto skill = make_shared<CItem_SkillMeteor>();
		skill->SetSpawnPos(Vec2(-100.f));

		m_Actors.push_back(gold);
		m_Actors.push_back(gold2);
		m_Actors.push_back(gold3);
		m_Actors.push_back(skill);
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


