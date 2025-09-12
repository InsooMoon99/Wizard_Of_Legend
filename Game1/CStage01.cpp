#include "stdafx.h"
#include "CStage01.h"

#include "CActor.h"
#include "CPlayer.h"

CStage01::CStage01()
{
	player = new CPlayer();

}

CStage01::~CStage01()
{
	Release();
}

void CStage01::Init()
{
	CScene::Init();
}

void CStage01::Update()
{
	CScene::Update();

	player->Tick();
}

void CStage01::LateUpdate()
{
	CScene::LateUpdate();

	
}

void CStage01::Render()
{
	CScene::Render();

	//cout << "스테이지 1" << endl;

	player->Render();
}

void CStage01::Release()
{
	CScene::Release();

	player->Release();
}
