#pragma once
#include "CComponent.h"

class CUI;
class CUI_PlayerHUD;
class CUI_BossHUD;
class CStatusComponent;

class CUIComponent : public CComponent
{
public:
	CUIComponent();
	virtual ~CUIComponent();

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	void SetPlayerStatus();
	void SetBossStatus();

	const vector<shared_ptr<CGameObject>>& GetHUD() { return m_HUD; }
	const shared_ptr<CUI_PlayerHUD> GetPlayerHUD() { return m_PlayerHUD; }
	const shared_ptr<CUI_BossHUD> GetBossHUD() { return m_BossHUD; }

private:
	vector<shared_ptr<CGameObject>> m_HUD;

	shared_ptr<CUI_PlayerHUD>	m_PlayerHUD;
	shared_ptr<CUI_BossHUD>		m_BossHUD;

};

