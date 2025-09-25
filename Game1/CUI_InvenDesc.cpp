#include "stdafx.h"
#include "CUI_InvenDesc.h"

CUI_InvenDesc::CUI_InvenDesc()
{
	Vec2 outRectSize = Vec2(128.f * 1.1f, 56.f) * 3.f;
	m_OutRect = RESOURCE->CreateUI(L"UI\\Inventory_OutRect.png", outRectSize, true);
	m_OutRect->SetWorldPos(Vec2(-400.f, -180.f));
	m_OutRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	Vec2 inRectSize = Vec2(131.f, 33.f) * 3.f;
	m_InRect = RESOURCE->CreateUI(L"UI\\Inventory_InRect.png", inRectSize, true);
	m_InRect->pivot = OFFSET_LT;
	m_InRect->SetWorldPos(Vec2(-595.f, -140.f));
	m_InRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	m_Desc_Panel = L"- Á¤º¸ -";
}

CUI_InvenDesc::~CUI_InvenDesc()
{
	Release();
}

void CUI_InvenDesc::BeginPlay()
{
	CUI::BeginPlay();

}

void CUI_InvenDesc::Tick()
{
	CUI::Tick();

	m_OutRect->Update();
	m_InRect->Update();
}

void CUI_InvenDesc::LateUpdate()
{
	CUI::LateUpdate();

	
}

void CUI_InvenDesc::Render()
{
	CUI::Render();

	DWRITE->RenderText(m_Desc_Panel, RECT{ 218, 470,450,520 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	m_OutRect->Render();
	m_InRect->Render();
}

void CUI_InvenDesc::Release()
{
	CUI::Release();

}
