#include "stdafx.h"
#include "CUIComponent.h"
#include "CResourceManager.h"

CUIComponent::CUIComponent()
{
	Resource_Settings();
}

CUIComponent::~CUIComponent()
{
}

void CUIComponent::BeginPlay()
{
	CComponent::BeginPlay();
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
		hud->Update();
}

void CUIComponent::Render()
{
	CComponent::Render();

	// ³ªÁß¿¡ Ã³¸®
	//DWRITE->RenderText(L"HP : ", RECT{ 150, 30,(long)app.GetWidth(),(long)app.GetHalfHeight() },
	//	25.0f, L"¸¼Àº °íµñ", Color(1, 1, 1, 1), DWRITE_FONT_WEIGHT_SEMI_BOLD);

	for (auto& hud : m_HUD)
		hud->Render();
}

void CUIComponent::Release()
{
	CComponent::Release();

	m_HUD.clear();
}

void CUIComponent::Resource_Settings()
{
	shared_ptr<ObImage> playerHUD = RESOURCE->CreateImage(L"UI\\UI_PLAYERBAR.bmp", Vec2(82.f, 20.f) * 4.f, true);
	playerHUD->SetWorldPos(Vec2(-450.f, 300.f));
	playerHUD->collider = COLLIDER::NONE;
	playerHUD->visible = true;
	playerHUD->space = SPACE::SCREEN;

	m_HUD.push_back(playerHUD);

	shared_ptr<ObImage> playerHp = RESOURCE->CreateImage(L"UI\\UI_HPBAR.bmp", Vec2(122.f, 16.f) * 2.f, true);
	playerHp->SetParentRT(*playerHUD);
	playerHp->SetLocalPos(Vec2(34.f, 12.f));
	playerHp->collider = COLLIDER::NONE;
	playerHp->visible = true;
	playerHp->space = SPACE::SCREEN;

	m_HUD.push_back(playerHp);

	shared_ptr<ObImage> playerMp = RESOURCE->CreateImage(L"UI\\UI_MANABAR.bmp", Vec2(96.f, 8.f) * 2.f, true);
	playerMp->SetParentRT(*playerHUD);
	playerMp->SetLocalPos(Vec2(9.f, -20.f));
	playerMp->collider = COLLIDER::NONE;
	playerMp->visible = true;
	playerMp->space = SPACE::SCREEN;

	m_HUD.push_back(playerMp);


}
