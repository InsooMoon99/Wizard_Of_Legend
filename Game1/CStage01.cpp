#include "stdafx.h"
#include "CStage01.h"

#include "CActor.h"
#include "CPlayer.h"
#include "CResourceManager.h"

CStage01::CStage01()
{
	m_Player = new CPlayer();

	m_MapSize = Vec2(1184.f, 688.f) * 5.f;

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

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	player->Get_PlayerCollision()->SetWorldPos(Vec2(-2300.f, -970.f));

}

void CStage01::Update()
{
	CScene::Update();

	m_Player->Tick();

	for (auto& map : m_Map)
	{
		map.second->Update();
	}
}

void CStage01::LateUpdate()
{
	CScene::LateUpdate();

	m_Player->LateUpdate();

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

	//cout << "스테이지 1" << endl;

	for (auto& map : m_Map)
	{
		map.second->Render();
	}

	m_Player->Render();
}

void CStage01::Release()
{
	CScene::Release();

	m_Player->Release();

	m_Map.clear();
}

void CStage01::Resource_Setting()
{
	// Map
	

	shared_ptr<ObImage> map = RESOURCE->CreateImage(
		L"Edit\\TutorialCourse.png", m_MapSize,true);

	map->SetWorldPos(Vec2(0,0));
	m_Map.insert({ ESceneType::Stage01, map });
}
