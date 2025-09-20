#include "stdafx.h"
#include "CUIComponent.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CUI.h"
#include "CUI_PlayerStatus.h"
#include "CStatusComponent.h"
#include "CUI_BossHUD.h"
#include "CUI_BossStatus.h"
#include "CUI_PlayerHUD.h"

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

void CUIComponent::SetPlayerStatus()
{
	if (GetOwner())
	{
		m_PlayerHUD = make_shared<CUI_PlayerHUD>();
		m_HUD.push_back(m_PlayerHUD);


		auto* status = CHelper::GetComponent<CStatusComponent>(GetOwner());
		if (status == nullptr)
			return;

		m_PlayerHUD->Get_Status()->SetStatus(status);
	}
}

void CUIComponent::SetBossStatus()
{
	if (GetOwner())
	{
		m_BossHUD = make_shared<CUI_BossHUD>();
		m_HUD.push_back(m_BossHUD);

		auto* status = CHelper::GetComponent<CStatusComponent>(GetOwner());
		if (status == nullptr)
			return;

		m_BossHUD->Get_Status()->SetStatus(status);
	}
}
