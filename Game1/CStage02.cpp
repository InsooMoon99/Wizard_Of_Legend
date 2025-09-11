#include "stdafx.h"
#include "CStage02.h"

CStage02::CStage02()
{
}

CStage02::~CStage02()
{
}

void CStage02::Init()
{
	CScene::Init();
}

void CStage02::Update()
{
	CScene::Update();
}

void CStage02::LateUpdate()
{
	CScene::LateUpdate();
}

void CStage02::Render()
{
	CScene::Render();

	cout << "스테이지 2요" << endl;
}

void CStage02::Release()
{
	CScene::Release();
}
