#pragma once
#include "CComponent.h"

class CUI;
class CUI_PlayerHUD;
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

	CUI_PlayerHUD* m_PlayerHUD;

};

