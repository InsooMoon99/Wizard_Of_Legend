#include "stdafx.h"
#include "CUI_HUD.h"

CUI_HUD::CUI_HUD()
{
}

CUI_HUD::~CUI_HUD()
{
	Release();
}

void CUI_HUD::BeginPlay()
{
	CUI::BeginPlay();

	for (auto& hud : m_Children)
	{
		hud->BeginPlay();
	}
}

void CUI_HUD::Tick()
{
	CUI::Tick();

	for (auto& hud : m_Children)
	{
		hud->Tick();
	}
}

void CUI_HUD::Render()
{
	CUI::Render();

	for (auto& hud : m_Children)
	{
		hud->Render();
	}
}

void CUI_HUD::Release()
{
	CUI::Release();

	m_Children.clear();
}

void CUI_HUD::AddChild(shared_ptr<CUI> _Ui)
{
	if (_Ui == nullptr)
		return;

	m_Children.push_back(_Ui);
}

bool CUI_HUD::RemoveChild(shared_ptr<CUI> _Ui)
{
	auto find_iter = find(m_Children.begin(), m_Children.end(), _Ui);

	if (find_iter == m_Children.end())
		return false;

	m_Children.erase(find_iter);

	return true;
}
