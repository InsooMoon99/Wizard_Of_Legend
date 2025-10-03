#include "stdafx.h"
#include "CUI_InvenStatus.h"

#include "CStatusComponent.h"

CUI_InvenStatus::CUI_InvenStatus()
{
	Vec2 outRectSize = Vec2(128.f * 1.1f, 40.f) * 3.f;
	m_OutRect = RESOURCE->CreateUI(L"UI\\Inventory_OutRect.png", outRectSize, true);
	m_OutRect->SetWorldPos(Vec2(-400.f, 38.f));
	m_OutRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	Vec2 inRectSize = Vec2(131.f, 22.f) * 3.f;
	m_InRect = RESOURCE->CreateUI(L"UI\\Inventory_InRect.png", inRectSize, true);
	m_InRect->pivot = OFFSET_LT;
	m_InRect->SetParentT(*m_OutRect);
	m_InRect->SetLocalPos(Vec2(-197.f, 15.f));
	m_InRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	m_Item_Panel = L"- 스테이터스 - ";
}

CUI_InvenStatus::~CUI_InvenStatus()
{
	Release();
}

void CUI_InvenStatus::BeginPlay()
{
	CUI::BeginPlay();


}

void CUI_InvenStatus::Tick()
{
	CUI::Tick();

	auto status = m_Status.lock();
	if (!status) return;

	m_Attack = status->GetAttack();
	m_Def = status->GetDef();

	m_OutRect->Update();
	m_InRect->Update();
}

void CUI_InvenStatus::LateUpdate()
{
	CUI::LateUpdate();

	
}

void CUI_InvenStatus::Render()
{
	CUI::Render();

	//if (!GetRender()) return;

	DWRITE->RenderText(m_Item_Panel, RECT{ 192, 280,450,330 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	DWRITE->RenderText(L"공격력 : " + to_wstring(m_Attack), RECT{ 118, 328,340,400 },
		17.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	DWRITE->RenderText(L"방어력 : " + to_wstring(m_Def), RECT{ 265, 328,440,400 },
		17.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	m_OutRect->Render();
	m_InRect->Render();
}

void CUI_InvenStatus::Release()
{
	CUI::Release();

}
