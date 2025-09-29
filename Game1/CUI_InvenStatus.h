#pragma once

#include "CUI.h"

class CStatusComponent;

class CUI_InvenStatus : public CUI
{
public:
	CUI_InvenStatus();
	virtual ~CUI_InvenStatus();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

	void SetStatus(shared_ptr<CStatusComponent> _Stats) { m_Status = _Stats; };

private:
	weak_ptr<CStatusComponent> m_Status;

	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	wstring m_Item_Panel;

	int m_Attack = 0;
	int m_Def = 0;
};

