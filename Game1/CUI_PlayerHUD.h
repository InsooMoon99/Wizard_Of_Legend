#pragma once

#include "CUI_HUD.h"

class CStatusComponent;

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

private:
	shared_ptr<CUI> m_PlayerStatus;

};

