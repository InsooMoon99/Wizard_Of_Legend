#include "stdafx.h"
#include "CStage01.h"
#include "CActor.h"
#include "CDummy.h"
#include "CPlayer.h"
#include "CResourceManager.h"

CStage01::CStage01()
{
	m_Player = new CPlayer();
	m_Actors.push_back(m_Player);

	m_TestDummy = new CDummy();
	m_Actors.push_back(m_TestDummy);

	m_MapSize = Vec2(1184.f, 688.f) * 4.f;
	m_PlayerPos = Vec2(-1790.f, -790.f);

	Resource_Setting();
}

CStage01::~CStage01()
{
	Release();
}

void CStage01::Init()
{
	CScene::Init();

	m_Player->BeginPlay();
	m_TestDummy->BeginPlay();

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	player->Get_PlayerCollision()->SetWorldPos(m_PlayerPos);

	CDummy* dummy = dynamic_cast<CDummy*>(m_TestDummy);
	dummy->Get_DummyCol()->SetWorldPos(m_PlayerPos + Vec2(250.f, 150.f));

}

void CStage01::Update()
{
	CScene::Update();


	for (auto& actor : m_Actors)
		actor->Tick();

	for (auto& map : m_Map)
		map.second->Update();

	for (auto& object : m_Objects)
		object->Update();
}

void CStage01::LateUpdate()
{
	CScene::LateUpdate();

	//m_Player->LateUpdate();

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);

	//플레이어 가두기 이건 딱히 필요 없을듯. 타일맵으로 충돌처리
	//player->Get_PlayerCollision()->SetWorldPosX(Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().x, -3800.0f * 1.5f / 2.0f, 3840.0f * 1.5f / 2.0f));
	//캠
	//CAM->position.x = Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().x, -m_MapSize.x + app.GetHalfWidth(), m_MapSize.x - app.GetHalfWidth());
	//CAM->position.y = Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().y + 500.0f, m_MapSize.y, 950.0f - app.GetHalfHeight());

	CAM->position = player->Get_PlayerCollision()->GetWorldPos();
}

void CStage01::Render()
{
	CScene::Render();

	for (auto& map : m_Map)
		map.second->Render();

	for (auto& object : m_Objects)
		object->Render();

	for (auto& actor : m_Actors)
		actor->Render();
}

void CStage01::Release()
{
	CScene::Release();

	//for (auto& actor : m_Actors)
	//	SafeDelete(actor);

	m_Map.clear();
}

void CStage01::Resource_Setting()
{
	// Map
	shared_ptr<ObImage> mapShadow = RESOURCE->CreateImage(L"Edit\\SHADOW.bmp", m_MapSize * 5.f, true);
	mapShadow->SetWorldPos(Vec2(0, 0));
	mapShadow->color = Color(0.0f, 0.0f, 0.0f, 1.0f);
	m_Map.insert({ ESceneType::Shadow, mapShadow });

	shared_ptr<ObImage> map = RESOURCE->CreateImage(L"Edit\\TutorialCourse.png", m_MapSize, true);
	map->SetWorldPos(Vec2(0, 0));
	m_Map.insert({ ESceneType::Stage01, map });

	shared_ptr<ObImage> teleport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", Vec2(311.f, 298.f), true);
	teleport->SetWorldPos(m_PlayerPos);
	m_Objects.push_back(teleport);

	

}