#include "stdafx.h"
#include "CUIComponent.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CUI.h"
#include "CUI_PlayerStatus.h"
#include "CStatusComponent.h"

CUIComponent::CUIComponent()
{
	m_PlayerHUD = new CUI_PlayerStatus();

	m_HUD.push_back(m_PlayerHUD);

}

CUIComponent::~CUIComponent()
{
}

void CUIComponent::BeginPlay()
{
	CComponent::BeginPlay();

	SetPlayerStatus();

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

	for (auto& hud : m_HUD)
		hud->Render();
}

void CUIComponent::Release()
{
	CComponent::Release();

	m_HUD.clear();
}

void CUIComponent::SetPlayerStatus()
{
	if (m_PlayerHUD && GetOwner())
	{
		CStatusComponent* status = CHelper::GetComponent<CStatusComponent>(GetOwner());
		if (status == nullptr)
			return;

		m_PlayerHUD->SetStatus(status);
	}
}
