#include "stdafx.h"
#include "CUIComponent.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CUI.h"
#include "CUI_PlayerStatus.h"
#include "CStatusComponent.h"
#include "CUI_BossHUD.h"
#include "CUI_BossStatus.h"
#include "CUI_InvenSkill.h"
#include "CUI_InventoryHUD.h"
#include "CUI_PlayerHUD.h"
#include "CUI_Skill_Pannel.h"
#include "CWeaponComponent.h"

CUIComponent::CUIComponent()
{
	
}

CUIComponent::~CUIComponent()
{
}

void CUIComponent::BeginPlay()
{
	CComponent::BeginPlay();

	for (auto& hud : m_HUD)
		hud->BeginPlay();
}

void CUIComponent::TickComponent()
{
	CComponent::TickComponent();

#ifdef _DEBUG
	//if (ImGui::Begin("UI"))
	//{
	//	Vec2 pos = m_HUD[1]->GetLocalPos();
	//	ImGui::InputFloat2("Hp Pos : ", &pos.x);
	//
	//	Vec2 pos2 = m_HUD[2]->GetLocalPos();
	//	ImGui::InputFloat2("Mp Pos : ", &pos2.x);
	//
	//	m_HUD[1]->SetLocalPos(pos);
	//	m_HUD[2]->SetLocalPos(pos2);
	//}
	//
	//ImGui::End();

#endif

	for (auto& hud : m_HUD)
		hud->Tick();
}

void CUIComponent::Render()
{
	CComponent::Render();

	//for (auto& hud : m_HUD)
	//	hud->Render();
}

void CUIComponent::Release()
{
	CComponent::Release();

	m_HUD.clear();
}

void CUIComponent::SetPlayerHUD()
{
	if (GetOwner())
	{
		m_PlayerHUD = make_shared<CUI_PlayerHUD>();
		//m_PlayerHUD->BeginPlay();

		m_HUD.push_back(m_PlayerHUD);

		auto status = CHelper::GetComponent<CStatusComponent>(GetOwner());
		if (status == nullptr) return;

		dynamic_pointer_cast<CUI_PlayerHUD>(m_PlayerHUD)->Get_Status()->SetStatus(status.get());

		auto weapon = CHelper::GetComponent<CWeaponComponent>(GetOwner());
		if (weapon == nullptr) return;

		dynamic_pointer_cast<CUI_PlayerHUD>(m_PlayerHUD)->Get_Panel()->SetWeapon(weapon);
	}
}

void CUIComponent::SetBossHUD()
{
	if (GetOwner())
	{
		m_BossHUD = make_shared<CUI_BossHUD>();
		//m_BossHUD->BeginPlay();

		m_HUD.push_back(m_BossHUD);

		auto status = CHelper::GetComponent<CStatusComponent>(GetOwner());
		if (status == nullptr)
			return;

		dynamic_pointer_cast<CUI_BossHUD>(m_BossHUD)->Get_Status()->SetStatus(status.get());
	}
}

void CUIComponent::SetInventoryHUD()
{
	if (GetOwner())
	{
		m_InventoryHUD = make_shared<CUI_InventoryHUD>();
		//m_InventoryHUD->BeginPlay();

		m_HUD.push_back(m_InventoryHUD);

		auto weapon = CHelper::GetComponent<CWeaponComponent>(GetOwner());
		if (weapon == nullptr)
			return;

		dynamic_pointer_cast<CUI_InventoryHUD>(m_InventoryHUD)->GetInvenSkill()->SetWeapon(weapon);
	}
}

void CUIComponent::ToggleInventory()
{
	bToggle = !bToggle;

	// 인벤토리 UI
	if (m_InventoryHUD) 
	{
		m_InventoryHUD->FOnRendering.Broadcast(bToggle);
		//m_InventoryHUD->SetRender(bToggle);
	}
}
