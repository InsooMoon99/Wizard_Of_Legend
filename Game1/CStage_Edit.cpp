#include "stdafx.h"
#include "CStage_Edit.h"
#include "CPlayer.h"
#include "CUIComponent.h"
#include "CTileManager.h"

CStage_Edit::CStage_Edit()
{
	m_Player = make_shared<CPlayer>();
	m_Actors.push_back(m_Player);

	m_MapSize = Vec2(2304.f, 2560.f) * 3.f;
	m_PlayerPos = Vec2(516.f, -2593.f);

	shared_ptr<ObImage> map = RESOURCE->CreateImage(L"Edit\\Fire_End.png", m_MapSize, true);
	map->SetLocalPos(Vec2(-185, 46));
	m_Map.insert({ ESceneType::Edit, map });
}

CStage_Edit::~CStage_Edit()
{
	Release();
}

void CStage_Edit::Init()
{
	CScene::Init();

	for (auto& actor : m_Actors)
		actor->BeginPlay();

	auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	player->Get_Collision()->SetWorldPos(m_PlayerPos);

}

void CStage_Edit::Update()
{
	CScene::Update();

	Key_Input();

	for (auto& actor : m_Actors)
		actor->Tick();

	for (auto& map : m_Map)
		map.second->Update();
}

void CStage_Edit::LateUpdate()
{
	CScene::LateUpdate();

	auto player = dynamic_pointer_cast<CPlayer>(m_Player);
	CAM->position = player->Get_Collision()->GetWorldPos();

}

void CStage_Edit::Render()
{
	CScene::Render();

	Vec2 pos = m_Map[ESceneType::Edit]->GetWorldPos();
	Vec2 size = m_MapSize;

	if (ImGui::Begin("Debug"))
	{
		Vec2 min_pos = -size * 0.5f;
		Vec2 max_pos = size * 0.5f;

		ImGui::SliderFloat2("Stage Position", (float*)&pos, -200.f, +200.f);

		//m_Map[ESceneType::Edit]->SetWorldPos(pos);
	}

	ImGui::End();

	for (auto& map : m_Map)
		map.second->Render();

	TILE->Render();

	for (auto& actor : m_Actors)
		actor->Render();

	
}

void CStage_Edit::Release()
{
	CScene::Release();

}

void CStage_Edit::Key_Input()
{
	if (INPUT->KeyDown(VK_LBUTTON))
	{
		Vec2 mousePos = INPUT->GetMouseWorldPos();
		TILE->Picking(mousePos, 1, 1, false);
	}

	if (INPUT->KeyPress(VK_LBUTTON))
	{
		Vec2 mousePos = INPUT->GetMouseWorldPos();

		int cx = (int)floorf(mousePos.x / TILECX);
		int cy = (int)floorf(mousePos.y / TILECY);

		if (m_LastPaintCell.x != cx || m_LastPaintCell.y != cy) 
		{
			TILE->Picking(mousePos, 0, 1, true);
			m_LastPaintCell = { cx, cy };
		}
	}

	if (INPUT->KeyPress(VK_RBUTTON))
	{
		TILE->Erase_Tile();
	}

	if (INPUT->KeyDown('T'))
		TILE->Save_Data();
	if (INPUT->KeyDown('L'))
	{
		//TILE->Load_Data(L"../Data/FireTile_First.dat");
		TILE->Load_Data(L"../Data/FireTile_Second.dat");
	}

	if (INPUT->KeyDown(VK_F1))
	{
		TILE->ToggleTileImagesVisible();
	}
		
}

