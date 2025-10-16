#include "stdafx.h"
#include "CStage01.h"
#include "CActor.h"
#include "CBoss_TwinMan.h"
#include "CDummy.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CStateComponent.h"
#include "CTestGhoul.h"
#include "CUIComponent.h"

CStage01::CStage01()
{
	m_Player = make_shared<CPlayer>();
	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player.get());
	m_TestDummy = make_shared<CDummy>();
	m_TestGhoul = make_shared<CTestGhoul>();

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

	auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	player->Get_Collision()->SetWorldPos(m_PlayerPos);

	auto dummy = dynamic_pointer_cast<CDummy>(m_TestDummy);
	dummy->Get_Collision()->SetWorldPos(m_PlayerPos + Vec2(250.f, 150.f));

	auto test = dynamic_pointer_cast<CTestGhoul>(m_TestGhoul);
	test->Get_Collision()->SetWorldPos(m_PlayerPos + Vec2(250.f, 0.f));
	test->SetTarget(player);

	m_Actors.push_back(test);

}

void CStage01::Update()
{
	CScene::Update();

	RENDER->Release();

	if (INPUT->KeyDown('L'))
	{
		//TILE->Load_Data();
	}

	for (auto& actor : m_Actors)
	{
		actor->Tick();
		RENDER->AddRenderObject(actor.get());
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

	auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	CAM->position = player->Get_Collision()->GetWorldPos();

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