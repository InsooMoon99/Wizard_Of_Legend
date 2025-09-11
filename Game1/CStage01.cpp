#include "stdafx.h"
#include "CStage01.h"

CStage01::CStage01()
{

}

CStage01::~CStage01()
{

}

void CStage01::Init()
{
	CScene::Init();
}

void CStage01::Update()
{
	CScene::Update();
}

void CStage01::LateUpdate()
{
	CScene::LateUpdate();

}

void CStage01::Render()
{
	CScene::Render();

	cout << "스테이지 1이요" << endl;
}

void CStage01::Release()
{
	CScene::Release();
}
