#include "stdafx.h"
#include "CStage_Start.h"

CStage_Start::CStage_Start()
{
	m_Title_Image = RESOURCE->CreateImage(L"Title\\Background0.png", Vec2(1280.0f, 800.f), true);
	m_Title_Text  = RESOURCE->CreateImage(L"Title\\TitleLogo.png", Vec2(1280.f, 800.f), true);
	m_Title_Text->SetLocalPos(Vec2(0.0f, 50.f));

	m_Test_Start = RESOURCE->CreateSprite(L"Title\\Start.bmp", Vec2(300.f / 2.f, 150.f), Int2(2,1), true);
	m_Test_Start->SetWorldPos(Vec2(-300.f, -250.f));
	m_Test_Start->space = SPACE::SCREEN;
	m_Test_Start->collider = COLLIDER::RECT;

	m_Test_Edit  = RESOURCE->CreateSprite(L"Title\\Edit.bmp", Vec2(300.f / 2.f, 150.f), Int2(2, 1), true);
	m_Test_Edit->SetWorldPos(Vec2(0.f, -250.f));
	m_Test_Edit->space = SPACE::SCREEN;
	m_Test_Edit->collider = COLLIDER::RECT;

	m_Test_Exit  = RESOURCE->CreateSprite(L"Title\\Exit.bmp", Vec2(300.f / 2.f, 150.f), Int2(2, 1), true);
	m_Test_Exit->SetWorldPos(Vec2(300.f, -250.f));
	m_Test_Exit->space = SPACE::SCREEN;
	m_Test_Exit->collider = COLLIDER::RECT;

	m_Images.push_back(m_Title_Image);
	m_Images.push_back(m_Title_Text);
	m_Images.push_back(m_Test_Start);
	m_Images.push_back(m_Test_Edit);
	m_Images.push_back(m_Test_Exit);

}

CStage_Start::~CStage_Start()
{
	Release();
}

void CStage_Start::Init()
{
	CScene::Init();

	SOUND->Play("Title");
	SOUND->SetVolume("Title", 0.1f);

}

void CStage_Start::Update()
{
	CScene::Update();

	for (auto& title : m_Images)
		title->Update();
}

void CStage_Start::LateUpdate()
{
	CScene::LateUpdate();

	if (!m_Test_Start && !m_Test_Edit && !m_Test_Exit) return;

	if (m_Test_Start->IntersectScreenMouse(INPUT->GetMouseScreenPos()))
	{
		m_Test_Start->frame.x = 1;

		if (INPUT->KeyDown(VK_LBUTTON))
		{
			SCENE->Change_Scene(ESceneType::Stage01);

			return;
		}
	}

	else
		m_Test_Start->frame.x = 0;

	if (m_Test_Edit->IntersectScreenMouse(INPUT->GetMouseScreenPos()))
	{
		m_Test_Edit->frame.x = 1;

		if (INPUT->KeyDown(VK_LBUTTON))
		{
			SCENE->Change_Scene(ESceneType::Edit);

			return;
		}
	}

	else
		m_Test_Edit->frame.x = 0;

	if (m_Test_Exit->IntersectScreenMouse(INPUT->GetMouseScreenPos()))
	{
		m_Test_Exit->frame.x = 1;

		if (INPUT->KeyDown(VK_LBUTTON))
		{
			PostQuitMessage(0);
		}
	}

	else
		m_Test_Exit->frame.x = 0;

}

void CStage_Start::Render()
{
	CScene::Render();

	for (auto& title : m_Images)
		title->Render();

}

void CStage_Start::Release()
{
	CScene::Release();

	m_Images.clear();

	SOUND->DeleteSound("Title");
}
