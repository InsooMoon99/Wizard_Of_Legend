#include "stdafx.h"
#include "CStage_Edit.h"
#include "CPlayer.h"
#include "CUIComponent.h"
#include "CTileManager.h"

CStage_Edit::CStage_Edit()
{
	m_Player = new CPlayer();

	m_Actors.push_back(m_Player);
}

CStage_Edit::~CStage_Edit()
{
	Release();
}

void CStage_Edit::Init()
{
	CScene::Init();

	TILE->Init();

	for (auto& actor : m_Actors)
		actor->BeginPlay();

}

void CStage_Edit::Update()
{
	CScene::Update();

	Key_Input();

	TILE->Update();

	for (auto& actor : m_Actors)
		actor->Tick();
}

void CStage_Edit::LateUpdate()
{
	CScene::LateUpdate();

	CPlayer* player = dynamic_cast<CPlayer*>(m_Player);
	CAM->position = player->Get_Collision()->GetWorldPos();


	TILE->LateUpdate();
}

void CStage_Edit::Render()
{
	CScene::Render();

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

		TILE->Picking(mousePos, 0, 1);
	}

	if (INPUT->KeyDown('S'))
	{
		
	}

	if (INPUT->KeyDown('L'))
	{

	}
}
