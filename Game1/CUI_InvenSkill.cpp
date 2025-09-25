#include "stdafx.h"
#include "CUI_InvenSkill.h"

CUI_InvenSkill::CUI_InvenSkill()
{
	m_Panel_Name = L"- 아르카나 - ";

	Vec2 outRectSize = Vec2(128.f * 1.1f, 56.f) * 3.f;
	m_OutRect = RESOURCE->CreateUI(L"UI\\Inventory_OutRect.png", outRectSize, true);
	m_OutRect->SetWorldPos(Vec2(-400.f, 180.f));
	m_OutRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	Vec2 inRectSize = Vec2(131.f, 22.f) * 3.f;
	m_InRect = RESOURCE->CreateUI(L"UI\\Inventory_InRect.png", inRectSize, true);
	m_InRect->pivot = OFFSET_LT;
	m_InRect->SetWorldPos(Vec2(-595.f, 220.f));
	m_InRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	m_Skill_Panel.resize(SKILL_PANEL);
	m_Skill_Key.resize(SKILL_PANEL);

	Vec2 panelSize = Vec2(42.f, 40.f);
	float padding = 10.f;
	float startX = 50.f; 

	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_Skill_Panel[i] = RESOURCE->CreateUI(L"UI\\Panel.png", panelSize, true);
		m_Skill_Panel[i]->SetParentT(*m_InRect);
		m_Skill_Panel[i]->pivot = OFFSET_LT;
		m_Skill_Panel[i]->SetLocalPos(Vec2(startX + i * (panelSize.x + padding), -20.f));
	}

	Vec2 keySize = Vec2(16.f, 16.f) * 1.5f;
	m_Skill_Key[0] = RESOURCE->CreateUI(L"Key\\M0.png", keySize, true);
	m_Skill_Key[1] = RESOURCE->CreateUI(L"Key\\Space.png", keySize, true);
	m_Skill_Key[2] = RESOURCE->CreateUI(L"Key\\M1.png", keySize, true);
	m_Skill_Key[3] = RESOURCE->CreateUI(L"Key\\Q.png", keySize, true);
	m_Skill_Key[4] = RESOURCE->CreateUI(L"Key\\E.png", keySize, true);
	m_Skill_Key[5] = RESOURCE->CreateUI(L"Key\\R.png", keySize, true);

	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_Skill_Key[i]->SetParentRT(*m_Skill_Panel[i]);
		m_Skill_Key[i]->SetLocalPos(Vec2(22.f, 15.f));
	}
}

CUI_InvenSkill::~CUI_InvenSkill()
{
	Release();
}

void CUI_InvenSkill::BeginPlay()
{
	CUI::BeginPlay();

}

void CUI_InvenSkill::Tick()
{
	CUI::Tick();

	m_OutRect->Update();
	m_InRect->Update();

	for (auto& panel : m_Skill_Panel)
	{
		panel->Update();
	}

	for (auto& key : m_Skill_Key)
	{
		key->Update();
	}
}

void CUI_InvenSkill::LateUpdate()
{
	CUI::LateUpdate();

	
}

void CUI_InvenSkill::Render()
{
	CUI::Render();

	DWRITE->RenderText(m_Panel_Name, RECT{ 200, 110,450,150 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	m_OutRect->Render();
	m_InRect->Render();

	for (auto& panel : m_Skill_Panel)
	{
		panel->Render();
	}


	for (auto& key : m_Skill_Key)
	{
		key->Render();
	}
}

void CUI_InvenSkill::Release()
{
	CUI::Release();

	m_Skill_Panel.clear();
}
