#pragma once

#include "CUI_HUD.h"

class CStatusComponent;
class CUI_PlayerStatus;
class CUI_Skill_Pannel;
class CUI_InventoryHUD;

class CUI_PlayerHUD : public CUI_HUD
{
	using Super = CUI_HUD;

public:
	CUI_PlayerHUD();
	virtual ~CUI_PlayerHUD();

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

public:
	shared_ptr<CUI_PlayerStatus> Get_Status() { return m_PlayerStatus; }
	shared_ptr<CUI_Skill_Pannel> Get_Panel() { return m_Skill_Panel; }

private:
	shared_ptr<CUI_PlayerStatus>	m_PlayerStatus;
	shared_ptr<CUI_Skill_Pannel>	m_Skill_Panel;

	shared_ptr<CUI_InventoryHUD>	m_Inventory;

};

