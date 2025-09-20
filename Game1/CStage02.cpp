#include "stdafx.h"
#include "CStage02.h"
#include "CActor.h"
#include "CBoss_Light.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CUIComponent.h"

CStage02::CStage02()
{
	m_Player = new CPlayer();
	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player);

	m_Boss = new CBoss_Light();
	m_BossUI = CHelper::GetComponent<CUIComponent>(m_Boss);

	m_Actors.push_back(m_Boss);
	m_Actors.push_back(m_Player);

	m_MapSize = Vec2(1568.f, 576.f) * 4.f;
	m_PlayerPos = Vec2(-1790.f, -790.f);

	Resource_Setting();
}

CStage02::~CStage02()
{
}

void CStage02::Init()
{
	CScene::Init();

	for (auto& actor : m_Actors)
		actor->BeginPlay();

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	player->Get_Collision()->SetWorldPos(m_PlayerPos);

	CBoss_Light* boss = dynamic_cast<CBoss_Light*>(m_Boss);

	Vec2 manBossPos = m_PlayerPos + Vec2(100.f, 200.f);
	Vec2 girlBossPos = manBossPos + Vec2(150.f, 0.f);

	// 각 보스의 위치를 CBoss_Light에 설정
	boss->Set_ManPos(manBossPos);
	boss->Set_GirlPos(girlBossPos);

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

	//for (auto& actor : m_Actors)
	//	actor->Render();

	RENDER->Render();
}

void CStage02::Release()
{
	CScene::Release();

	m_Map.clear();
}

void CStage02::Resource_Setting()
{
	// Map
	shared_ptr<ObImage> mapShadow = RESOURCE->CreateImage(L"Edit\\SHADOW.bmp", m_MapSize * 5.f, true);
	mapShadow->SetWorldPos(Vec2(0, 0));
	mapShadow->color = Color(0.0f, 0.0f, 0.0f, 1.0f);
	m_Map.insert({ ESceneType::Shadow, mapShadow });

	shared_ptr<ObImage> map = RESOURCE->CreateImage(L"Edit\\TutorialCourse.png", m_MapSize, true);
	map->SetWorldPos(Vec2(0, 0));
	map->SetLocalPos(Vec2(0, 30));
	m_Map.insert({ ESceneType::Stage02, map });

	shared_ptr<ObImage> teleport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", Vec2(311.f, 298.f), true);
	teleport->SetWorldPos(m_PlayerPos);
	m_Objects.push_back(teleport);
}
