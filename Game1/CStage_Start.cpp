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

	// TODO : 여기서 Game Start, Edit, Exit 세개 고를 수 있도록 만들기
}

void CStage_Start::Release()
{
	CScene::Release();
}
