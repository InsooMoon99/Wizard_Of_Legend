#include "stdafx.h"
#include "CStageFire.h"
#include "CActor.h"
#include "CBlobRoller.h"
#include "CBoss_TwinMan.h"
#include "CDummy.h"
#include "CMonsterBase.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CShopNPC.h"
#include "CStateComponent.h"
#include "CUIComponent.h"

CStageFire::CStageFire()
{
	m_MapSize = Vec2(2304.f, 2560.f) * 3.f;
	m_PlayerPos = Vec2(516.f, -2593.f);

	Resource_Setting();
	m_Actors.push_back(m_Player);

	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player.get());

	bStartTile = false;

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

	PARTICLE->Spawn_Particle(L"StartEffect", Vec2(m_PlayerPos.x, m_PlayerPos.y + 180.f), Vec2(1.2f, 1.f));

	for (auto& actor : m_Actors)
		actor->BeginPlay();

	//auto dummy = dynamic_pointer_cast<CDummy>(m_TestDummy);
	//dummy->Get_Collision()->SetWorldPos(m_PlayerPos + Vec2(150.f, 50.f));

	//auto roller = dynamic_pointer_cast<CBlobRoller>(blobRoller);
	//roller->Get_Collision()->SetWorldPos(m_PlayerPos + Vec2(150.f, 50.f));

	bStartTile = true;
	  
	if (bStartTile)
	{
		TILE->Load_Data(L"../Data/FireTile_First.dat");
		bStartTile = false;
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

	//m_Player->LateUpdate();

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
	if (INPUT->KeyDown(VK_F1))
	{
		TILE->ToggleTileImagesVisible();
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

	m_Player	= make_shared<CPlayer>();
	m_Player->SetSpawnPos(m_Section1_Pos);

	m_TestDummy = make_shared<CDummy>();
	m_TestDummy->SetSpawnPos(m_PlayerPos + Vec2(150.f, 50.f));

	m_ShopNPC	= make_shared<CShopNPC>();

	blobRoller = make_shared<CBlobRoller>();
	blobRoller->SetSpawnPos(m_Section1_Pos);

	m_Actors.push_back(m_ShopNPC);
	m_Actors.push_back(m_TestDummy);
	m_Actors.push_back(blobRoller);
}


