#pragma once

#include "CUI_HUD.h"

class CStatusComponent;
class CUI_BossStatus;

class CUI_BossHUD : public CUI_HUD
{
	using Super = CUI_HUD;

public:
	CUI_BossHUD();
	virtual ~CUI_BossHUD();

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

public:
	shared_ptr<CUI_BossStatus> Get_Status() { return m_BossStatus;; }

	bool bRender = false;

private:
	shared_ptr<CUI_BossStatus> m_BossStatus;


};

