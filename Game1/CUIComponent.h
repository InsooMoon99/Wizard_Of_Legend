#pragma once
#include "CComponent.h"

class CUI;
class CUI_PlayerStatus;
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

private:
	vector<CUI*> m_HUD;
	CUI_PlayerStatus* m_PlayerHUD;

};

