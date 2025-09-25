#pragma once

#include "CUI_HUD.h"

class CStatusComponent;
class CUI_PlayerStatus;
class CUI_Skill_Pannel;
class CUI_Inventory;

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

private:
	shared_ptr<CUI_PlayerStatus> m_PlayerStatus;
	shared_ptr<CUI_Skill_Pannel> m_Skill_Pannel;

	shared_ptr<CUI_Inventory>	 m_Inventory;

};

