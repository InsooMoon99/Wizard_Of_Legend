#include "stdafx.h"
#include "CUI_BossStatus.h"

#include "CStatusComponent.h"

CUI_BossStatus::CUI_BossStatus()
{
	m_Background = RESOURCE->CreateUI(L"UI\\BOSS_HP_BACK.png", Vec2(114.f, 16.f) * 2.8f, true);
	m_Background->SetWorldPos(Vec2(0.f, 280.f));
	m_Background->collider = COLLIDER::NONE;
	m_Background->space = SPACE::SCREEN;

	m_Hp = RESOURCE->CreateUI(L"UI\\BOSS_HPBAR.png", Vec2(m_HpScale.x, m_HpScale.y), true);
	m_Hp->SetParentRT(*m_Background);
	m_Hp->SetLocalPos(Vec2(-123.5f, -1.8f));
	m_Hp->pivot = OFFSET_L;
	m_Hp->collider = COLLIDER::NONE;
	m_Hp->space = SPACE::SCREEN;

	m_BossStatus.push_back(m_Background);
	m_BossStatus.push_back(m_Hp);
}

CUI_BossStatus::~CUI_BossStatus()
{
	Release();
}

void CUI_BossStatus::BeginPlay()
{
	CUI::BeginPlay();
}

void CUI_BossStatus::Tick()
{
	CUI::Tick();

	if (m_Status)
	{
		float hpRatio = m_Status->GetHp() / m_Status->GetMaxHp();
		m_Hp->scale.x = m_HpScale.x * hpRatio;
	
	}

	for (auto& status : m_BossStatus)
	{
		status->Update();
	}
}

void CUI_BossStatus::Render()
{
	CUI::Render();

//#ifdef _DEBUG
//	if (ImGui::Begin("Debug"))
//	{
//		Vec2 pos = m_Background->GetLocalPos();
//		ImGui::InputFloat2("BossBack", &pos.x);
//	
//		m_Background->SetLocalPos(pos);
//	}
//	
//	ImGui::End();
//#endif

	wstring bossName = L"ÃµµÕ¹ø°³ ½ÖµÕÀÌ";

	DWRITE->RenderText(bossName, RECT{ 550, 25,750,50 },
		25.0f, L"¸¼Àº °íµñ", Color(1, 1, 1, 1), DWRITE_FONT_WEIGHT_BOLD);


	for (auto& status : m_BossStatus)
	{
		status->Render();
	}
}

void CUI_BossStatus::Release()
{
	CUI::Release();

	m_BossStatus.clear();
}
