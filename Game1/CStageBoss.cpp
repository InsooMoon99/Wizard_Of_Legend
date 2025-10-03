#include "stdafx.h"
#include "CStageBoss.h"
#include "CActor.h"
#include "CBoss_Controller.h"
#include "CBoss_TwinGirl.h"
#include "CBoss_TwinMan.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CUIComponent.h"

CStageBoss::CStageBoss()
{
	m_PlayerPos = Vec2(m_StartPos.x, m_StartPos.y + 100.f);

	m_Player = make_shared<CPlayer>();
	m_Player->SetSpawnPos(m_PlayerPos);
	m_PlayerUI = CHelper::GetComponent<CUIComponent>(m_Player.get());
	
	m_ReadyRect = make_shared<ObRect>();
	m_ReadyRect->SetWorldPos(m_ReadyPos);
	m_ReadyRect->visible = false;
	m_ReadyRect->isFilled = false;
	m_ReadyRect->scale = Vec2(400.f, 200.f);
	m_ReadyRect->collider = COLLIDER::RECT;

	m_MapSize = Vec2(1280.f, 1280.f) * 3.f;

	Resource_Setting();
}

CStageBoss::~CStageBoss()
{
	Release();
}

void CStageBoss::Init()
{
	CScene::Init();

	PARTICLE->Spawn_ParticleDir(L"StartEffect", Vec2(m_PlayerPos.x, m_PlayerPos.y + 180.f), Vec2(1.4f, 1.f));
	SOUND->StopPlay("Teleport", 0.2f);

	SOUND->Play("BossMap");
	SOUND->SetVolume("BossMap", 0.1f);

	m_BossController = new CBoss_Controller();
	m_BossUI = CHelper::GetComponent<CUIComponent>(m_BossController);

	m_Actors.push_back(m_BossController);

	m_Boss_Man  = new CBoss_TwinMan(m_BossController);
	m_Boss_Girl = new CBoss_TwinGirl(m_BossController);
	// 플레이어 세팅
	m_Boss_Man->SetPlayer(m_Player.get());
	m_Boss_Girl->SetPlayer(m_Player.get());

	m_Actors.push_back(m_Boss_Man);
	m_Actors.push_back(m_Boss_Girl);
	m_Actors.push_back(m_Player.get());

	//auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	//player->Get_Collision()->SetWorldPos(m_PlayerPos);

	Vec2 m_BossPos = Vec2(-1815.f, 100.f);
	m_Boss_Man->Get_Collision()->SetWorldPos(m_BossPos);
	m_Boss_Girl->Get_Collision()->SetWorldPos(Vec2(m_BossPos.x + 50, m_BossPos.y));

	m_Boss_Man->Set_WorldPos(m_BossPos);
	m_Boss_Girl->Set_WorldPos(Vec2(m_BossPos.x + 50, m_BossPos.y));

	for (auto& actor : m_Actors)
		actor->BeginPlay();
}

void CStageBoss::Update()
{
	CScene::Update();

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

	m_ReadyRect->Update();
}

void CStageBoss::LateUpdate()
{
	CScene::LateUpdate();

	auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	CAM->position = player->Get_Collision()->GetWorldPos();

	if (m_ReadyRect->Intersect(m_Player->Get_Collision().get()))
	{
		IsCameraSat = true;

		if (IsFirstCheck)
		{
			IsFirstCheck = false;

			TILE->Load_Data(L"../Data/FireTile_Boss.dat");

			m_BossController->ChangeState(ECharacterState::Boss_Start);

			if (!m_BlendToClamp) { m_BlendToClamp = true; m_ClampBlendT = 0.0f; }
		}
	}

	if (IsCameraSat)
	{
		Vec2 freeCam = player->Get_Collision()->GetWorldPos();

		const float minX = -2605.f, maxX = -937.f;
		const float yA = 542.f, yB = -382.f;
		const float minY = min(yA, yB), maxY = max(yA, yB);

		float halfW = app.GetHalfWidth();
		float halfH = app.GetHalfHeight();

		Vec2 hard = freeCam; 
		if ((maxX - minX) <= halfW * 2.f)
			hard.x = (minX + maxX) * 0.5f;

		else                              
			hard.x = Utility::Saturate(freeCam.x, minX + halfW, maxX - halfW);

		if ((maxY - minY) <= halfH * 2.f) 
			hard.y = (minY + maxY) * 0.5f;

		else                               
			hard.y = Utility::Saturate(freeCam.y, minY + halfH, maxY - halfH);

		if (m_BlendToClamp && m_ClampBlendT < 1.0f)
		{
			m_ClampBlendT += DELTA / max(m_ClampBlendTime, 1e-4f);
			float t = clamp(m_ClampBlendT, 0.f, 1.f);
			float s = t * t * (3.f - 2.f * t);

			CAM->position = Utility::Lerp(freeCam, hard, s);

			if (t >= 1.f) m_BlendToClamp = false; 
		}
		else
		{
			CAM->position = hard;
		}
	}
	else
	{
		CAM->position = player->Get_Collision()->GetWorldPos();
	}
}

void CStageBoss::Render()
{
	CScene::Render();

	for (auto& map : m_Map)
		map.second->Render();

	TILE->Render();

	for (auto& object : m_Objects)
		object->Render();

	m_ReadyRect->Render();

	PARTICLE->Render();
	RENDER->Render();
}

void CStageBoss::Release()
{
	CScene::Release();

	m_Map.clear();
}

void CStageBoss::Resource_Setting()
{
	// Map
	shared_ptr<ObImage> mapShadow = RESOURCE->CreateImage(L"Edit\\stage0_bg.png", Vec2(1920.f, 1080.f) * 4.f, true);
	mapShadow->SetWorldPos(m_PlayerPos);
	m_Map.insert({ ESceneType::Shadow, mapShadow });

	Vec2 mapPos = Vec2(-1780.f + 225.f, -800.f);
	shared_ptr<ObImage> map = RESOURCE->CreateImage(L"Edit\\BossMap_Remove.png", m_MapSize, true);
	map->SetWorldPos(mapPos);
	m_Map.insert({ ESceneType::StageBoss, map });

	shared_ptr<ObImage> teleport = RESOURCE->CreateImage(L"Object\\TELEPORT.png", Vec2(311.f, 298.f), true);
	teleport->SetWorldPos(Vec2(m_StartPos.x, m_StartPos.y + 100.f));
	m_Objects.push_back(teleport);
}
