#include "stdafx.h"
#include "CUI_InvenSkill.h"
#include "CWeaponComponent.h"
#include "CSkillDataManager.h"

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
	m_Skill_Panel_Gold.resize(SKILL_PANEL);
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
		m_Skill_Panel[i]->collider = COLLIDER::RECT;
	}

	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_Skill_Panel_Gold[i] = RESOURCE->CreateUI(L"UI\\GoldPanelUnfilled.png", panelSize * 1.2f, false);
		m_Skill_Panel_Gold[i]->SetParentT(*m_InRect);
		m_Skill_Panel_Gold[i]->pivot = OFFSET_LT;
		m_Skill_Panel_Gold[i]->SetLocalPos(Vec2(startX + i * (panelSize.x + padding) - 2.f, -18.f));
	}

	Vec2 keySize = Vec2(16.f, 16.f) * 1.5f;
	m_Skill_Key[LButton] = RESOURCE->CreateUI(L"Key\\M0.png", keySize, true);
	m_Skill_Key[Space] = RESOURCE->CreateUI(L"Key\\Space.png", keySize, true);
	m_Skill_Key[RButton] = RESOURCE->CreateUI(L"Key\\M1.png", keySize, true);
	m_Skill_Key[Q] = RESOURCE->CreateUI(L"Key\\Q.png", keySize, true);
	m_Skill_Key[E] = RESOURCE->CreateUI(L"Key\\E.png", keySize, true);
	m_Skill_Key[R] = RESOURCE->CreateUI(L"Key\\R.png", keySize, true);

	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_Skill_Key[i]->SetParentRT(*m_Skill_Panel[i]);
		m_Skill_Key[i]->SetLocalPos(Vec2(22.f, 15.f));
	}

	bRender = false;
}

CUI_InvenSkill::~CUI_InvenSkill()
{
	Release();


}

void CUI_InvenSkill::BeginPlay()
{
	CUI::BeginPlay();

	FOnRendering.Add([this](bool bCheck)
		{
			this->SetRendering(bCheck);
		});

	m_Skill_Panel_Gold[LButton]->visible = true;
}

void CUI_InvenSkill::Tick()
{
	CUI::Tick();

	// TODO : 슬롯의 스킬이 바꼈으면, UI 최신화 해줘야함
	auto weapon = m_Weapon.lock();
	if (weapon)
	{
		auto infos = weapon->GetSkill_UiInfo();

		if(infos != m_Skill_Infos)
		{
			m_Skill_Infos = std::move(infos);

			// 기존 클론 삭제하기
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
				clone->SetLocalPos(Vec2(1.f, -1.f));
				clone->visible = GetRender();

				m_IconClones[idx] = std::move(clone);
			}
		}
		
	}

	OnIntersectMouse();

	m_OutRect->Update();
	m_InRect->Update();

	for (auto& panel : m_Skill_Panel)
		panel->Update();

	for (auto& panel : m_Skill_Panel_Gold)
		panel->Update();

	for (auto& key : m_Skill_Key)
		key->Update();

	for (auto clone : m_IconClones)
	{
		if (clone)
			clone->Update();
	}
}
	
void CUI_InvenSkill::LateUpdate()
{
	CUI::LateUpdate();

}

void CUI_InvenSkill::Render()
{
	CUI::Render();

	if (!GetRender()) return;

	DWRITE->RenderText(m_Panel_Name, RECT{ 200, 110,450,150 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	m_OutRect->Render();
	m_InRect->Render();

	for (auto& panel : m_Skill_Panel)
		panel->Render();

	for (auto& panel : m_Skill_Panel_Gold)
		panel->Render();

	for (auto& key : m_Skill_Key)
		key->Render();

	for (auto& clone : m_IconClones)
	{
		if (clone)
			clone->Render();
	}

}

void CUI_InvenSkill::Release()
{
	CUI::Release();

	m_Skill_Panel.clear();
	m_Skill_Key.clear();

	// 참조 해제
	m_Weapon.reset();
}

void CUI_InvenSkill::SetRendering(bool _Render)
{
	CUI::SetRendering(_Render);

	for (auto& clone : m_IconClones)
	{
		if (clone)
			clone->visible = _Render;
	}
}


void CUI_InvenSkill::OnIntersectMouse()
{
	if (!GetRender()) return;
	
	int newHovered = -1;
	Vec2 mouse = INPUT->GetMouseScreenPos();
	
    for (int i = 0; i < SKILL_PANEL; ++i)
    {
        auto panel = m_Skill_Panel[i];
        auto gold_panel  = m_Skill_Panel_Gold[i];
        if (!panel || !gold_panel) 
			continue;

        bool cood = panel->IntersectScreenMouse(mouse);

        if (cood)
		{
			gold_panel->visible = true;
            newHovered = i;
            break; 
        }

        else
        {
			gold_panel->visible = false;
        }
    }
	
	if (newHovered != m_LastHovered)
	{
		if (newHovered != -1)
		{
			//SOUND->StopPlay("Select", 0.5f);
			FOnSkillIntersectMouse.Broadcast(static_cast<ESkillSlot>(newHovered));
		}
		m_LastHovered = newHovered;
	}
}


