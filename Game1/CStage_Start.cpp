#include "stdafx.h"
#include "CStage_Start.h"

CStage_Start::CStage_Start()
{
	m_Title_Image = RESOURCE->CreateImage(L"Title\\Background0.png", Vec2(1280.0f, 800.f), true);
	m_Title_Shadow = RESOURCE->CreateImage(L"Back.bmp", Vec2(1920.f, 1440.f), false);
	m_Title_Shadow->color = Color(0.5f, 0.5f, 0.5f, 0.25f);

	shared_ptr<ObRect> rect1 = make_shared<ObRect>();
	rect1->SetWorldPos(Vec2(16.f, -65.f));
	rect1->scale = Vec2(200.f, 50.f);
	rect1->isFilled = false;
	rect1->visible = false;
	rect1->collider = COLLIDER::RECT;

	shared_ptr<ObRect> rect2 = make_shared<ObRect>();
	rect2->SetWorldPos(Vec2(16.f, -135.f));
	rect2->scale = Vec2(200.f, 50.f);
	rect2->isFilled = false;
	rect2->visible = false;
	rect2->collider = COLLIDER::RECT;

	shared_ptr<ObRect> rect3 = make_shared<ObRect>();
	rect3->SetWorldPos(Vec2(16.f, -205.f));
	rect3->scale = Vec2(200.f, 50.f);
	rect3->isFilled = false;
	rect3->visible = false;
	rect3->collider = COLLIDER::RECT;

	rect1->space = SPACE::SCREEN;
	rect2->space = SPACE::SCREEN;
	rect3->space = SPACE::SCREEN;

	m_TextRect.push_back(rect1);
	m_TextRect.push_back(rect2);
	m_TextRect.push_back(rect3);

	m_Title_Text  = RESOURCE->CreateImage(L"Title\\TitleLogo.png", Vec2(1280.f, 800.f), true);
	m_Title_Text->SetLocalPos(Vec2(0.0f, -50.f));

	m_Images.push_back(m_Title_Image);
	m_Images.push_back(m_Title_Shadow);
	m_Images.push_back(m_Title_Text);

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

	m_Settings.push_back(EStartStageType::GameStart);
	m_Settings.push_back(EStartStageType::Editor);
	m_Settings.push_back(EStartStageType::Exit);

	m_DrawRect[(int)EStartStageType::Pressed] = RECT(460.f, 550.f, WINCX, WINCY);
	m_DrawRect[(int)EStartStageType::GameStart] = RECT(580.f, 400.f, WINCX, WINCY);
	m_DrawRect[(int)EStartStageType::Editor] = RECT(580.f, 470.f, WINCX, WINCY);
	m_DrawRect[(int)EStartStageType::Exit] = RECT(580.f, 540.f, WINCX, WINCY);

	for (auto& color : m_ChangeColor)
	{
		color = Color(1, 1, 1, 1);
	}
}

void CStage_Start::Update()
{
	CScene::Update();

	for (auto& title : m_Images)
		title->Update();

	for (auto& text : m_TextRect)
		text->Update();

}

void CStage_Start::LateUpdate()
{
	CScene::LateUpdate();

	if (INPUT->KeyDown(VK_RETURN))
	{
		SOUND->StopPlay("Select", 0.5f);

		if (StartType == EStartStageType::Pressed)
		{
			bStartCheck = true;
			m_Title_Shadow->visible = true;
		}
	}
	if (bStartCheck == true)
	{
		m_UpTimer += DELTA;

		if (m_UpTimer < 0.3f)
		{
			m_Title_Text->MoveWorldPos(UP * 500.f * DELTA);
		}

		if (m_UpTimer > 0.3f)
		{
			bVisible = true;
		}

		// 마우스 충돌
		for (int i = 0; i < 3; i++)
		{
			if (m_TextRect[i]->IntersectScreenMouse(INPUT->GetMouseScreenPos()))
			{
				if (INPUT->KeyDown(VK_LBUTTON))
				{
					switch (i)
					{
					case 0:
						SCENE->Change_Scene(ESceneType::StageFire);
						return;

					case 1:
						SCENE->Change_Scene(ESceneType::Edit);
						return;

					case 2:
						PostQuitMessage(0);
						break;
					}
				}

				m_ChangeColor[i] = Color(1, 1, 1, 1);
			}

			else
			{
				m_ChangeColor[i] = Color(0.3, 0.3, 0.3, 1);
			}
		}
		
	}

	
}

void CStage_Start::Render()
{
	CScene::Render();

	if (bStartCheck == false)
		CHelper::OutlineText(L"아무 버튼이나 누르세요", m_DrawRect[(int)EStartStageType::Pressed], 35.f, m_Font, Color(1,1,1,1));

	if (bStartCheck && bVisible)
	{
		CHelper::OutlineText(L"게임 시작", m_DrawRect[(int)EStartStageType::GameStart], 35.f, m_Font, m_ChangeColor[0]);
		CHelper::OutlineText(L"맵 에디터", m_DrawRect[(int)EStartStageType::Editor], 35.f, m_Font, m_ChangeColor[1]);
		CHelper::OutlineText(L"게임 종료", m_DrawRect[(int)EStartStageType::Exit], 35.f, m_Font, m_ChangeColor[2]);
	}

	for (auto& title : m_Images)
		title->Render();

	for (auto& text : m_TextRect)
		text->Render();
}

void CStage_Start::Release()
{
	CScene::Release();

	m_Images.clear();
	m_TextRect.clear();
	m_Settings.clear();

	SOUND->DeleteSound("Title");
}
