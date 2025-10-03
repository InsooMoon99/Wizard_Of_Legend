#include "stdafx.h"
#include "CUI_InvenCloth.h"

CUI_InvenCloth::CUI_InvenCloth() : m_Cloth_Panel(L"희망")
{
	Vec2 outRectSize = Vec2(128.f * 1.1f, 26.f) * 3.f;
	m_OutRect = RESOURCE->CreateUI(L"UI\\Inventory_OutRect.png", outRectSize, true);
	m_OutRect->SetWorldPos(Vec2(-400.f, 305.f));
	m_OutRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	Vec2 inRectSize = Vec2(131.f, 18.f) * 3.f;
	m_InRect = RESOURCE->CreateUI(L"UI\\Inventory_InRect.png", inRectSize, true);
	m_InRect->pivot = OFFSET_LT;
	m_InRect->SetWorldPos(Vec2(-595.f, 330.f));
	m_InRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	m_ClothBack = RESOURCE->CreateUI(L"UI\\ClothBackGround.png", Vec2(44.f, 44.f) * 1.4f, true);
	m_ClothBack->SetParentT(*m_OutRect);
	m_ClothBack->SetLocalPos(Vec2(-100.f, 0.f));

	m_Cloth = RESOURCE->CreateUI(L"UI\\Cloth.png", Vec2(32.f, 32.f) * 3.f, true);
	m_Cloth->SetParentT(*m_OutRect);
	m_Cloth->SetLocalPos(Vec2(-100.f, 0.f));

	
}

CUI_InvenCloth::~CUI_InvenCloth()
{
	Release();
}

void CUI_InvenCloth::BeginPlay()
{
	CUI::BeginPlay();

}

void CUI_InvenCloth::Tick()
{
	CUI::Tick();

	m_OutRect->Update();
	m_InRect->Update();

	m_ClothBack->Update();
	m_Cloth->Update();
}

void CUI_InvenCloth::LateUpdate()
{
	CUI::LateUpdate();

	
}

void CUI_InvenCloth::Render()
{
	CUI::Render();

	DWRITE->RenderText(m_Cloth_Panel, RECT{ 218, 44,450,330 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	m_OutRect->Render();
	m_InRect->Render();

	m_ClothBack->Render();
	m_Cloth->Render();
}

void CUI_InvenCloth::Release()
{
	CUI::Release();

}
