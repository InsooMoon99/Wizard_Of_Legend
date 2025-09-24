#include "stdafx.h"
#include "CStage02.h"
#include "CActor.h"
#include "CBoss_Controller.h"
#include "CBoss_TwinGirl.h"
#include "CBoss_TwinMan.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CUIComponent.h"

CStage02::CStage02()
{
	m_Player = new CPlayer();
	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player);

	m_MapSize = Vec2(1280.f, 1280.f) * 3.f;
	m_PlayerPos = Vec2(-1780.f, 000.f);

	Resource_Setting();
}

CStage02::~CStage02()
{

}

void CStage02::Init()
{
	CScene::Init();

	SOUND->Play("BossMap");
	SOUND->SetVolume("BossMap", 0.1f);

	m_BossController = new CBoss_Controller();
	m_BossUI = CHelper::GetComponent<CUIComponent>(m_BossController);

	m_Actors.push_back(m_BossController);

	m_Boss_Man  = new CBoss_TwinMan(m_BossController);
	m_Boss_Girl = new CBoss_TwinGirl(m_BossController);
	// 플레이어 세팅
	m_Boss_Man->SetPlayer(m_Player);
	m_Boss_Girl->SetPlayer(m_Player);

	m_Actors.push_back(m_Boss_Man);
	m_Actors.push_back(m_Boss_Girl);
	m_Actors.push_back(m_Player);

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	player->Get_Collision()->SetWorldPos(m_PlayerPos);

	Vec2 m_BossPos = Vec2(-1815.f, 200.f);
	m_Boss_Man->Get_Collision()->SetWorldPos(m_BossPos);
	m_Boss_Girl->Get_Collision()->SetWorldPos(Vec2(m_BossPos.x + 50, m_BossPos.y));

	m_Boss_Man->Set_WorldPos(m_BossPos);
	m_Boss_Girl->Set_WorldPos(Vec2(m_BossPos.x + 50, m_BossPos.y));

	for (auto& actor : m_Actors)
		actor->BeginPlay();
}

void CStage02::Update()
{
	CScene::Update();

	// 기존 Render 큐 비우기
	RENDER->Release(); 

	for (auto& actor : m_Actors)
	{
		actor->Tick();
		RENDER->AddRenderObject(actor);
	}

	for (const auto& hud : m_BossUI->GetHUD())
		RENDER->AddRenderObject(hud.get());

	for (const auto& hud : m_PlayerUI->GetHUD())
		RENDER->AddRenderObject(hud.get());

	for (auto& map : m_Map)
		map.second->Update();

	for (auto& object : m_Objects)
		object->Update();
}

void CStage02::LateUpdate()
{
	CScene::LateUpdate();

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	CAM->position = player->Get_Collision()->GetWorldPos();
}

void CStage02::Render()
{
	CScene::Render();

	for (auto& map : m_Map)
		map.second->Render();

	TILE->Render();

	for (auto& object : m_Objects)
		object->Render();

	RENDER->Render();
	PARTICLE->Render();
}

void CStage02::Release()
{
	CScene::Release();

	m_Map.clear();
}

void CStage02::Resource_Setting()
{
	// Map
	shared_ptr<ObImage> mapShadow = RESOURCE->CreateImage(L"Edit\\stage1_skyMove.bmp", m_MapSize * 5.f, true);
	mapShadow->SetWorldPos(Vec2(0, 0));
	m_Map.insert({ ESceneType::Shadow, mapShadow });

	Vec2 mapPos = Vec2(-1780.f + 225.f, -800.f);
	shared_ptr<ObImage> map = RESOURCE->CreateImage(L"Edit\\BossMap_Remove.png", m_MapSize, true);
	map->SetWorldPos(mapPos);
	m_Map.insert({ ESceneType::Stage02, map });

	shared_ptr<ObImage> teleport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", Vec2(311.f, 298.f), true);
	teleport->SetWorldPos(m_PlayerPos);
	//m_Objects.push_back(teleport);
}
