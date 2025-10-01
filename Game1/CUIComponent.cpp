#include "stdafx.h"
#include "CUIComponent.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CInventoryComponent.h"
#include "CUI.h"
#include "CUI_PlayerStatus.h"
#include "CStatusComponent.h"
#include "CUI_BossHUD.h"
#include "CUI_BossStatus.h"
#include "CUI_InvenDesc.h"
#include "CUI_InvenItem.h"
#include "CUI_InvenSkill.h"
#include "CUI_InvenStatus.h"
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

		m_HUD.push_back(m_InventoryHUD);

		auto weapon = CHelper::GetComponent<CWeaponComponent>(GetOwner());
		if (weapon == nullptr) return;

		auto status = CHelper::GetComponent<CStatusComponent>(GetOwner());
		if (status == nullptr) return;

		auto inven = CHelper::GetComponent<CInventoryComponent>(GetOwner());
		if (inven == nullptr) return;

		dynamic_pointer_cast<CUI_InventoryHUD>(m_InventoryHUD)->GetInvenSkill()->SetWeapon(weapon);
		dynamic_pointer_cast<CUI_InventoryHUD>(m_InventoryHUD)->GetInvenDesc()->SetWeapon(weapon);
		dynamic_pointer_cast<CUI_InventoryHUD>(m_InventoryHUD)->GetInvenStatus()->SetStatus(status);
		dynamic_pointer_cast<CUI_InventoryHUD>(m_InventoryHUD)->GetInvenItem()->SetInven(inven);
		dynamic_pointer_cast<CUI_InventoryHUD>(m_InventoryHUD)->GetInvenDesc()->SetInventory(inven);
	}
}

void CUIComponent::ToggleInventory()
{
	bToggle = !bToggle;

	if (bToggle)
		SOUND->StopPlay("InvenOn", 0.5f);
	else
		SOUND->StopPlay("InvenOff", 0.5f);

	// 인벤토리 UI, 델리게이트로 변경
	if (m_InventoryHUD) 
	{
		m_InventoryHUD->FOnRendering.Broadcast(bToggle);
		//m_InventoryHUD->SetRender(bToggle);
	}

	// 인벤토리랑 반대로
	if (m_PlayerHUD)
		m_PlayerHUD->FOnRendering.Broadcast(!bToggle);
}
