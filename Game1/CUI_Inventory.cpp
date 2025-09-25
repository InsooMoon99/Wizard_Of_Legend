#include "stdafx.h"
#include "CUI_Inventory.h"
#include "CResourceManager.h"

CUI_Inventory::CUI_Inventory()
{
	Vec2 backgroundSize = Vec2(300.f, 65.f);
	shared_ptr<ObImage> background = RESOURCE->CreateUI(L"UI\\ItemInfoPanel.png", backgroundSize, true);
	background->SetWorldPos(Vec2(-450.f, 300.f));

	m_Inventory.push_back(background);
}

CUI_Inventory::~CUI_Inventory()
{
	Release();
}

void CUI_Inventory::BeginPlay()
{
	CUI::BeginPlay();
}

void CUI_Inventory::Tick()
{
	CUI::Tick();

	#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::NewLine();
		Vec2 pos = m_Inventory[0]->GetWorldPos();
		ImGui::InputFloat2("Inventory : ", &pos.x);
	
		m_Inventory[0]->SetWorldPos(pos);
	}
	
	ImGui::End();
#endif

	for (auto& inven : m_Inventory)
	{
		inven->Update();
	}
}

void CUI_Inventory::LateUpdate()
{
	CUI::LateUpdate();
}

void CUI_Inventory::Render()
{
	CUI::Render();

	for (auto& inven : m_Inventory)
	{
		inven->Render();
	}
}

void CUI_Inventory::Release()
{
	CUI::Release();


	m_Inventory.clear();

}
