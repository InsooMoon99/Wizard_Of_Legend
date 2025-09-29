#include "stdafx.h"
#include "CUI_InvenDesc.h"

#include "CInventoryComponent.h"
#include "CWeaponComponent.h"

CUI_InvenDesc::CUI_InvenDesc()
{
	Vec2 outRectSize = Vec2(128.f * 1.1f, 56.f) * 3.f;
	m_OutRect = RESOURCE->CreateUI(L"UI\\Inventory_OutRect.png", outRectSize, true);
	m_OutRect->SetWorldPos(Vec2(-400.f, -270.f));
	m_OutRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	Vec2 inRectSize = Vec2(131.f, 33.f) * 3.f;
	m_InRect = RESOURCE->CreateUI(L"UI\\Inventory_InRect.png", inRectSize, true);
	m_InRect->pivot = OFFSET_LT;
	m_InRect->SetParentT(*m_OutRect);
	m_InRect->SetLocalPos(Vec2(-197.f, 35.f));
	m_InRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	m_Skill_Infos.resize(SKILL_PANEL);

	m_Panel = L"- Á¤º¸ -";
}

CUI_InvenDesc::~CUI_InvenDesc()
{
	Release();
}

void CUI_InvenDesc::BeginPlay()
{
	CUI::BeginPlay();

	auto weapon = m_Weapon.lock();
	if (weapon)
		m_Skill_Infos = weapon->GetSkill_UiInfo();

	auto inven = m_Inven.lock();
	if (inven)
		m_Item_Infos = inven->GetItem_UiInfo(5);

	m_NameRect = CHelper::DrawLTRect(Vec2(57.f, 602.f), Vec2(350.f, 50.f));
	m_DescRect = CHelper::DrawLTRect(Vec2(57.f, 630.f), Vec2(350.f, 50.f));
}

void CUI_InvenDesc::Tick()
{
	CUI::Tick();

	auto weapon = m_Weapon.lock();
	if (weapon)
	{
		auto infos = weapon->GetSkill_UiInfo();
		if (infos != m_Skill_Infos)
			m_Skill_Infos = std::move(infos);
	}

	auto inven = m_Inven.lock();
	if (inven)
	{
		auto infos = inven->GetItem_UiInfo(5);
		m_Item_Infos = std::move(infos);
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

	DWRITE->RenderText(m_Panel, RECT{ 208, 560,450,620 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	if (m_DescMode == EDescMode::Item) 
		RenderItemDesc();

	else   
		RenderSkillDesc();
}

void CUI_InvenDesc::Release()
{
	CUI::Release();

}

void CUI_InvenDesc::OnSkillMouseIntersect(ESkillSlot index)
{
	m_DescMode = EDescMode::Skill;
	m_SelectSlot = index;
}

void CUI_InvenDesc::OnItemMouseIntersect(int index)
{
	m_DescMode = EDescMode::Item;
	m_SelectItemSlot = index;
}

void CUI_InvenDesc::RenderSkillDesc()
{
	if (m_Skill_Infos.empty()) return;

	const auto& ui = m_Skill_Infos[m_SelectSlot];
	if (ui.Id.empty()) 
	{
		DWRITE->RenderText(L"[ºó ½½·Ô]", RECT{ 57,602,420,662 }, 16.f,
			L"Noto Sans Cond Med", Color(1, 1, 1, 1));
		return;
	}

	DWRITE->RenderText(ui.Name, m_NameRect, 17.f, L"Noto Sans Cond Med", Color(1, 1, 0, 1));
	DWRITE->RenderText(ui.Desc, m_DescRect, 16.f, L"Noto Sans Cond Med", Color(0.9f, 0.9f, 0.9f, 1));
}

void CUI_InvenDesc::RenderItemDesc()
{
	if (m_Item_Infos.empty()) return;

	const auto& ui = m_Item_Infos[m_SelectItemSlot];
	if (ui.IsEmpty || ui.Id.empty()) 
	{
		DWRITE->RenderText(L"[ºó ½½·Ô]", RECT{ 57,602,420,662 }, 16.f,
			L"Noto Sans Cond Med", Color(1, 1, 1, 1));
		return;
	}

	DWRITE->RenderText(ui.Name, m_NameRect, 17.f, L"Noto Sans Cond Med", Color(1, 1, 0, 1));
	DWRITE->RenderText(ui.Desc, m_DescRect, 16.f, L"Noto Sans Cond Med", Color(0.9f, 0.9f, 0.9f, 1));

}
