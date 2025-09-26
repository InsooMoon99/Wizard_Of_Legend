#include "stdafx.h"
#include "CUI_InvenItem.h"

CUI_InvenItem::CUI_InvenItem()
{
	Vec2 outRectSize = Vec2(128.f * 1.1f, 56.f) * 3.f;
	m_OutRect = RESOURCE->CreateUI(L"UI\\Inventory_OutRect.png", outRectSize, true);
	m_OutRect->SetWorldPos(Vec2(-400.f, 10.f));
	m_OutRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	Vec2 inRectSize = Vec2(131.f, 22.f) * 3.f;
	m_InRect = RESOURCE->CreateUI(L"UI\\Inventory_InRect.png", inRectSize, true);
	m_InRect->pivot = OFFSET_LT;
	m_InRect->SetWorldPos(Vec2(-595.f, 50.f));
	m_InRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	m_Item_Panel = L"- À¯¹° - ";
}

CUI_InvenItem::~CUI_InvenItem()
{
	Release();
}

void CUI_InvenItem::BeginPlay()
{
	CUI::BeginPlay();

}

void CUI_InvenItem::Tick()
{
	CUI::Tick();

	m_OutRect->Update();
	m_InRect->Update();
}

void CUI_InvenItem::LateUpdate()
{
	CUI::LateUpdate();

	
}

void CUI_InvenItem::Render()
{
	CUI::Render();

	DWRITE->RenderText(m_Item_Panel, RECT{ 218, 280,450,330 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	m_OutRect->Render();
	m_InRect->Render();
}

void CUI_InvenItem::Release()
{
	CUI::Release();

}
