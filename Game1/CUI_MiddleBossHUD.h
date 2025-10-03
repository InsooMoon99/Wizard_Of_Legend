#pragma once

#include "CUI_HUD.h"

class CStatusComponent;
class CUI_BossStatus;

class CUI_MiddleBossHUD : public CUI_HUD
{
	using Super = CUI_HUD;

public:
	CUI_MiddleBossHUD();
	virtual ~CUI_MiddleBossHUD();

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

public:
	shared_ptr<class CUI_MiddleBossStatus> Get_Status() { return m_BossStatus;; }

private:
	shared_ptr<CUI_MiddleBossStatus> m_BossStatus;


};

