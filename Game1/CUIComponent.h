#pragma once
#include "CComponent.h"

class CUI;
class CUI_PlayerHUD;
class CUI_BossHUD;
class CUI_InventoryHUD;
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
	void SetPlayerHUD();
	void SetBossHUD();
	void SetInventoryHUD();

	const vector<shared_ptr<CGameObject>>& GetHUD() { return m_HUD; }
	const shared_ptr<CUI> GetPlayerHUD()  { return m_PlayerHUD; }
	const shared_ptr<CUI> GetBossHUD() { return m_BossHUD; }
	const shared_ptr<CUI> GetInventoryHUD() { return m_InventoryHUD; }

	void ToggleInventory();

private:
	vector<shared_ptr<CGameObject>> m_HUD;

	shared_ptr<CUI>		m_PlayerHUD;
	shared_ptr<CUI>		m_BossHUD;
	shared_ptr<CUI>		m_InventoryHUD;

	bool bToggle = false;

};

