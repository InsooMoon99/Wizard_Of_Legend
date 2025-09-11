#include "stdafx.h"
#include "CStage_Start.h"

CStage_Start::CStage_Start()
{
}

CStage_Start::~CStage_Start()
{
}

void CStage_Start::Init()
{
	CScene::Init();
}

void CStage_Start::Update()
{
	CScene::Update();
}

void CStage_Start::LateUpdate()
{
	CScene::LateUpdate();

}

void CStage_Start::Render()
{
	CScene::Render();

	cout << "스테이지 스타트요" << endl;
}

void CStage_Start::Release()
{
	CScene::Release();
}
