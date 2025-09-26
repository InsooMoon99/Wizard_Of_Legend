#include "stdafx.h"
#include "CUI_Skill_Pannel.h"
#include "CResourceManager.h"
#include "CWeaponComponent.h"

CUI_Skill_Pannel::CUI_Skill_Pannel()
{
	m_Panel = make_shared<ObRect>();
	m_Panel->scale = Vec2(500.f, 120.f);
	m_Panel->pivot = OFFSET_LT;
	m_Panel->visible = false;
	m_Panel->isFilled = false;
	m_Panel->space = SPACE::SCREEN;
	m_Panel->collider = COLLIDER::NONE;
	m_Panel->SetLocalPos(Vec2(-600.f, -230.f));

	m_Skill_Panel.resize(SKILL_PANEL);
	m_Skill_Key.resize(SKILL_PANEL);

	Vec2 panelSize = Vec2(42.f, 40.f) * 1.3f;
	float padding = 10.f;
	float startX  = 10.f;

	for (int i = 0; i < SKILL_PANEL; i++)
	{
		m_Skill_Panel[i] = RESOURCE->CreateUI(L"UI\\Panel.png", panelSize, true);
		m_Skill_Panel[i]->SetParentT(*m_Panel);
		m_Skill_Panel[i]->pivot = OFFSET_LT;
		m_Skill_Panel[i]->SetLocalPos(Vec2(startX + i * (panelSize.x + padding), -48.f));
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

	bRender = true;
}

CUI_Skill_Pannel::~CUI_Skill_Pannel()
{
	Release();
}

void CUI_Skill_Pannel::BeginPlay()
{
	CUI::BeginPlay();

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

	// TODO : 슬롯의 스킬이 바꼈으면, UI 최신화 해줘야함
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
				clone->SetLocalPos(Vec2(8.f, -6.f));
				clone->visible = true;

				m_IconClones[idx] = std::move(clone);
			}
		}

	}

	m_Panel->Update();

	for (auto& panel : m_Skill_Panel)
	{
		panel->Update();
	}

	for (auto& key : m_Skill_Key)
	{
		key->Update();
	}

	for (auto clone : m_IconClones)
	{
		if (clone)
			clone->Update();
	}

}

void CUI_Skill_Pannel::Render()
{
	CUI::Render();

	m_Panel->Render();

	for (auto& panel : m_Skill_Panel)
		panel->Render();

	for (auto& key : m_Skill_Key)
		key->Render();

	for (auto& clone : m_IconClones)
	{
		if (clone)
			clone->Render();
	}
		
}

void CUI_Skill_Pannel::Release()
{
	CUI::Release();

	m_Skill_Panel.clear();
	m_Skill_Key.clear();
	m_Skill_Infos.clear();

	m_Weapon.reset();

}
