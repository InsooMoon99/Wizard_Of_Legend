#include "stdafx.h"
#include "CUI_Skill_Pannel.h"
#include "CResourceManager.h"

CUI_Skill_Pannel::CUI_Skill_Pannel()
{
	m_Pannel = RESOURCE->CreateImage(L"UI\\UI_SKILLBAR.bmp", Vec2(320.f, 80.f) * 1.5f, true);
	m_Pannel->SetWorldPos(Vec2(-380.f, -300.f));
	m_Pannel->collider = COLLIDER::NONE;
	m_Pannel->space = SPACE::SCREEN;

	m_Skill_UI.push_back(m_Pannel);
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

#ifdef _DEBUG
	if (ImGui::Begin("UI"))
	{
		Vec2 temp = m_Pannel->GetWorldPos();
		if(ImGui::InputFloat2("Pannel", &temp.x))
		{
			m_Pannel->SetWorldPos(temp);
		}
	}

	ImGui::End();
#endif


	for(auto& skill : m_Skill_UI)
	{
		skill->Update();
	}

}

void CUI_Skill_Pannel::Render()
{
	CUI::Render();

	for (auto& skill : m_Skill_UI)
	{
		skill->Render();
	}
}

void CUI_Skill_Pannel::Release()
{
	CUI::Release();

	m_Skill_UI.clear();
}
