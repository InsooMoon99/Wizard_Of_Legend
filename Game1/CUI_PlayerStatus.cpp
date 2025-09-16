#include "stdafx.h"
#include "CUI_PlayerStatus.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"

CUI_PlayerStatus::CUI_PlayerStatus()
{
	m_Background = RESOURCE->CreateImage(L"UI\\UI_PLAYERBAR.bmp", Vec2(82.f, 20.f) * 4.f, true);
	m_Background->SetWorldPos(Vec2(-450.f, 300.f));
	m_Background->collider = COLLIDER::NONE;
	m_Background->space = SPACE::SCREEN;

	m_PlayerStatus.push_back(m_Background);

	m_PlayerHp = RESOURCE->CreateImage(L"UI\\UI_HPBAR.bmp", Vec2(m_HpScaleX, 16.f) * 2.f, true);
	m_PlayerHp->SetParentRT(*m_Background);
	m_PlayerHp->SetLocalPos(Vec2(-88.f, 12.f));
	m_PlayerHp->pivot = OFFSET_L;
	m_PlayerHp->collider = COLLIDER::NONE;
	m_PlayerHp->visible = true;
	m_PlayerHp->space = SPACE::SCREEN;

	m_PlayerStatus.push_back(m_PlayerHp);

	m_PlayerMp = RESOURCE->CreateImage(L"UI\\UI_MANABAR.bmp", Vec2(96.f, 8.f) * 2.f, true);
	m_PlayerMp->SetParentRT(*m_Background);
	m_PlayerMp->SetLocalPos(Vec2(-88.f, -20.f));
	m_PlayerMp->pivot = OFFSET_L;
	m_PlayerMp->collider = COLLIDER::NONE;
	m_PlayerMp->visible = true;
	m_PlayerMp->space = SPACE::SCREEN;

	m_PlayerStatus.push_back(m_PlayerMp);
}

CUI_PlayerStatus::~CUI_PlayerStatus()
{
	Release();
}

void CUI_PlayerStatus::BeginPlay()
{
	CUI::BeginPlay();

}

void CUI_PlayerStatus::Tick()
{
	CUI::Tick();

	if (m_Status)
	{
		float hpRatio = m_Status->GetHp() / m_Status->GetMaxHp();
		m_PlayerHp->scale.x = m_HpScaleX * hpRatio;

		float mpRatio = m_Status->GetMp() / m_Status->GetMaxMp();
		m_PlayerMp->scale.x = m_MpScaleX * mpRatio;
	}


	for (auto& status : m_PlayerStatus)
	{
		status->Update();
	}

}

void CUI_PlayerStatus::Render()
{
	CUI::Render();

	for (auto& status : m_PlayerStatus)
	{
		status->Render();
	}
}

void CUI_PlayerStatus::Release()
{
	CUI::Release();

	m_PlayerStatus.clear();
}
