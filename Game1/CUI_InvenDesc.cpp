#include "stdafx.h"
#include "CUI_InvenDesc.h"

#include "CWeaponComponent.h"

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

	m_UiInfos.resize(SKILL_PANEL);

	m_Panel = L"- Á¤º¸ -";
}

CUI_InvenDesc::~CUI_InvenDesc()
{
	Release();
}

void CUI_InvenDesc::BeginPlay()
{
	CUI::BeginPlay();

	if (m_Weapon)
		m_UiInfos = m_Weapon->GetSkill_UiInfo();

	m_NameRect = CHelper::DrawLTRect(Vec2(57.f, 512.f), Vec2(350.f, 50.f));
	m_DescRect = CHelper::DrawLTRect(Vec2(57.f, 540.f), Vec2(350.f, 50.f));
}

void CUI_InvenDesc::Tick()
{
	CUI::Tick();

	if (m_Weapon)
	{
		auto infos = m_Weapon->GetSkill_UiInfo();
		if (infos != m_UiInfos)
			m_UiInfos = std::move(infos);
	}


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

	m_OutRect->Render();
	m_InRect->Render();

	DWRITE->RenderText(m_Panel, RECT{ 218, 470,450,520 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	if (m_UiInfos.empty()) return;

	const auto& ui = m_UiInfos[m_SelectSlot];
	if (ui.Id.empty())
	{
		DWRITE->RenderText(L"[ºó ½½·Ô]", RECT{ 57,512,420,560 }, 16.f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));
		return;
	}

	DWRITE->RenderText(ui.Name, m_NameRect, 16.f, L"Noto Sans Cond Med", Color(1, 1, 0, 1));
	DWRITE->RenderText(ui.Desc, m_DescRect, 14.f, L"Noto Sans Cond Med", Color(0.9f, 0.9f, 0.9f, 1));


}

void CUI_InvenDesc::Release()
{
	CUI::Release();

}

void CUI_InvenDesc::OnMouseIntersect(ESkillSlot index)
{
	m_SelectSlot = index;
}
