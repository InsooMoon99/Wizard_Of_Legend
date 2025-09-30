#include "stdafx.h"
#include "CUI_Skill_Pannel.h"
#include "CResourceManager.h"
#include "CWeaponComponent.h"

CUI_Skill_Pannel::CUI_Skill_Pannel()
{
	m_PanelRect = make_shared<ObRect>();
	m_PanelRect->scale = Vec2(500.f, 120.f);
	m_PanelRect->pivot = OFFSET_LT;
	m_PanelRect->visible = false;
	m_PanelRect->isFilled = false;
	m_PanelRect->space = SPACE::SCREEN;
	m_PanelRect->collider = COLLIDER::NONE;
	m_PanelRect->SetLocalPos(Vec2(-600.f, -230.f));

	m_Skill_Panel.resize(SKILL_PANEL);
	m_Skill_Key.resize(SKILL_PANEL);
	m_CoolTime_Rect.resize(SKILL_PANEL);

	Vec2 panelSize = Vec2(42.f, 40.f) * 1.3f;
	float padding = 10.f;
	float startX  = 10.f;

	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_Skill_Panel[i] = RESOURCE->CreateUI(L"UI\\Panel.png", panelSize, true);
		m_Skill_Panel[i]->SetParentT(*m_PanelRect);
		m_Skill_Panel[i]->pivot = OFFSET_LT;
		m_Skill_Panel[i]->SetLocalPos(Vec2(startX + i * (panelSize.x + padding), -48.f));
	}
	
	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_CoolTime_Rect[i] = make_shared<ObRect>();
		m_CoolTime_Rect[i]->scale = panelSize;
		m_CoolTime_Rect[i]->visible = true;
		m_CoolTime_Rect[i]->isFilled = true;
		m_CoolTime_Rect[i]->color = Color(0, 0, 0, 0.2);
		m_CoolTime_Rect[i]->SetParentT(*m_PanelRect);
		m_CoolTime_Rect[i]->pivot = OFFSET_LB;
		m_CoolTime_Rect[i]->space = SPACE::SCREEN;
		m_CoolTime_Rect[i]->SetLocalPos(Vec2(startX + i * (panelSize.x + padding), -48.f - panelSize.y));
	}

	Vec2 keySize = Vec2(16.f, 16.f) * 2.f;
	m_Skill_Key[LButton] = RESOURCE->CreateUI(L"Key\\M0.png", keySize, true);
	m_Skill_Key[Space] = RESOURCE->CreateUI(L"Key\\Space.png", keySize, true);
	m_Skill_Key[RButton] = RESOURCE->CreateUI(L"Key\\M1.png", keySize, true);
	m_Skill_Key[Q] = RESOURCE->CreateUI(L"Key\\Q.png", keySize, true);
	m_Skill_Key[E] = RESOURCE->CreateUI(L"Key\\E.png", keySize, true);
	m_Skill_Key[R] = RESOURCE->CreateUI(L"Key\\R.png", keySize, true);

	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_Skill_Key[i]->SetParentRT(*m_Skill_Panel[i]);
		m_Skill_Key[i]->SetLocalPos(Vec2(26.f, 15.f));
	}

	// RECT rect = { 66, 642, 126, 702 };
	int addOffset = 65;
	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_WritePos[i] = 
		{
			56 + (i * addOffset),
			645,
			126 + (i * addOffset),
			702};
	}

	bRender = true;
}

CUI_Skill_Pannel::~CUI_Skill_Pannel()
{
	Release();
}

void CUI_Skill_Pannel::BeginPlay()
{
	CUI::BeginPlay();

	FOnRendering.Add([this](bool bCheck)
		{
			SetRendering(bCheck);
		});
}

void CUI_Skill_Pannel::Tick()
{
	CUI::Tick();

	//#ifdef _DEBUG
	//	if (ImGui::Begin("Debug"))
	//	{
	//		Vec2 temp = m_Panel->GetWorldPos(); //
	//		if(ImGui::InputFloat2("Pannel", &temp.x))
	//		{
	//			m_Panel->SetWorldPos(temp);
	//		}
	//	}
	//
	//	ImGui::End();
	//#endif

	auto weapon = m_Weapon.lock();
	if (weapon)
	{
		auto infos = weapon->GetSkill_UiInfo();

		if (infos != m_Skill_Infos)
		{
			m_Skill_Infos = std::move(infos);

			for (auto& clone : m_IconClones)
				clone.reset();

			for (auto& info : m_Skill_Infos)
			{
				if (!info.Icon) continue;

				int idx = static_cast<int>(info.Slot);
				if (idx < 0 || idx >= SKILL_PANEL) continue;

				auto clone = make_shared<ObImage>(*info.Icon);
				clone->space = SPACE::SCREEN;
				clone->SetParentRT(*m_Skill_Panel[idx]);
				clone->pivot = OFFSET_LT;
				clone->SetLocalPos(Vec2(6.f, -6.f));
				clone->visible = true;

				m_IconClones[idx] = std::move(clone);
			}
		}
	}

	if (weapon)
	{
		auto infos = weapon->GetSkill_UiInfo();

		for (int i = 0; i < SKILL_PANEL; i++)
		{
			auto& rect = m_CoolTime_Rect[i];
			rect->visible = false;

			if (i < (int)infos.size())
			{
				auto& info = infos[i];

				if (info.IsCoolTime && info.Cooldown > 0.f)
				{
					float ratio = Utility::Saturate(info.CooldownRemain / info.Cooldown);

					float height = (m_Skill_Panel[i]->scale.y) * ratio;

					rect->scale.y = height;
					rect->visible = (height > 0.0001f);
				}
			}
		}
	}

	m_PanelRect->Update();

	for (auto& panel : m_Skill_Panel)
		panel->Update();

	for (auto& key : m_Skill_Key)
		key->Update();

	for (auto& coolRect : m_CoolTime_Rect)
		coolRect->Update();

	for (auto clone : m_IconClones)
		if (clone) clone->Update();

}

void CUI_Skill_Pannel::Render()
{
	CUI::Render();

	m_PanelRect->Render();

	if (bRender)
	{
		for (int i = 0; i < SKILL_PANEL; i++)
		{
			auto& info = m_Skill_Infos[i];

			if (info.IsCoolTime && info.CooldownRemain > 0.f)
			{
				double coolTime = info.CooldownRemain;
				wstring string = format(L"{:.1f}", coolTime);       

				DWRITE->RenderText(string, m_WritePos[i], 25.f, L"맑은 고딕", Color(1, 1, 1, 1), DWRITE_FONT_WEIGHT_BOLD);

			}
		}
	}

	for (auto& panel : m_Skill_Panel)
		panel->Render();

	for (auto& key : m_Skill_Key)
		key->Render();

	for (auto& clone : m_IconClones)
		if (clone) clone->Render();

	for (auto& coolRect : m_CoolTime_Rect)
		coolRect->Render();
		
}

void CUI_Skill_Pannel::Release()
{
	CUI::Release();

	m_Skill_Panel.clear();
	m_Skill_Key.clear();
	m_Skill_Infos.clear();

	m_Weapon.reset();

}
