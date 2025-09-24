#include "stdafx.h"
#include "CStage01.h"
#include "CActor.h"
#include "CBoss_TwinMan.h"
#include "CDummy.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CStateComponent.h"
#include "CUIComponent.h"

CStage01::CStage01()
{
	m_Player = new CPlayer();
	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player);
	m_TestDummy = new CDummy();

	m_Actors.push_back(m_TestDummy);
	m_Actors.push_back(m_Player);

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

	SOUND->Play("Tutorial");
	SOUND->SetVolume("Tutorial", 0.05f);

	for (auto& actor : m_Actors)
		actor->BeginPlay();

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	player->Get_Collision()->SetWorldPos(m_PlayerPos);

	CDummy* dummy = dynamic_cast<CDummy*>(m_TestDummy);
	dummy->Get_Collision()->SetWorldPos(m_PlayerPos + Vec2(250.f, 150.f));

}

void CStage01::Update()
{
	CScene::Update();

	RENDER->Release();

	if (INPUT->KeyDown('L'))
	{
		TILE->Load_Data();
	}

	for (auto& actor : m_Actors)
	{
		actor->Tick();
		RENDER->AddRenderObject(actor);
	}

	for (const auto& hud : m_PlayerUI->GetHUD())
		RENDER->AddRenderObject(hud.get());

	for (auto& map : m_Map)
	{
		map.second->Update();
	}
	
	for (auto& object : m_Objects)
		object->Update();

}

void CStage01::LateUpdate()
{
	CScene::LateUpdate();

	//m_Player->LateUpdate();

	//플레이어 가두기 이건 딱히 필요 없을듯. 타일맵으로 충돌처리
	//player->Get_PlayerCollision()->SetWorldPosX(Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().x, -3800.0f * 1.5f / 2.0f, 3840.0f * 1.5f / 2.0f));
	//캠
	//CAM->position.x = Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().x, -m_MapSize.x + app.GetHalfWidth(), m_MapSize.x - app.GetHalfWidth());
	//CAM->position.y = Utility::Saturate(player->Get_PlayerCollision()->GetWorldPos().y + 500.0f, m_MapSize.y, 950.0f - app.GetHalfHeight());

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	CAM->position = player->Get_Collision()->GetWorldPos();

	if (player->Get_Collision()->Intersect(m_TestDummy->Get_Collision().get()))
	{
		cout << "충돌띠";

		auto state = CHelper::GetComponent<CStateComponent>(player);
		if (state->Get_State() != ECharacterState::Hit)
		{
			state->Change_State(ECharacterState::Hit);
		}
	}

	if (m_Objects[1]->Intersect(m_Player->Get_Collision().get()))
	{
		SCENE->Change_Scene(ESceneType::Stage02);
	}
}

void CStage01::Render()
{
	CScene::Render();

	for (auto& map : m_Map)
		map.second->Render();
	
	TILE->Render();

	PARTICLE->Render();

	for (auto& object : m_Objects)
		object->Render();

	//for (auto& actor : m_Actors)
	//	actor->Render();

	RENDER->Render();
}

void CStage01::Release()
{
	CScene::Release();

	//for (auto& actor : m_Actors)
	//	SafeDelete(actor);

	m_Map.clear();

	SOUND->DeleteSound("Tutorial");
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
	map->SetLocalPos(Vec2(0, 30));
	m_Map.insert({ ESceneType::Stage01, map });

	shared_ptr<ObImage> teleport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", Vec2(311.f, 298.f), true);
	teleport->SetWorldPos(m_PlayerPos);
	m_Objects.push_back(teleport);

	Vec2 vortexSize = Vec2(280.f / 7.f * 5.f, 40.f * 6.f);
	shared_ptr<ObImage> vortex = RESOURCE->CreateSprite(L"Object\\Gravity_vortex_Loop.png", vortexSize, Int2(7,1), true);
	vortex->SetWorldPos(Vec2(-130.f, 0.f));
	vortex->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	vortex->collider = COLLIDER::CIRCLE;
	m_Objects.push_back(vortex);

}