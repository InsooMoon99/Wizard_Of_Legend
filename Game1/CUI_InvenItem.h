#pragma once

#include "CUI.h"

class CUI_InvenItem : public CUI
{
public:
	CUI_InvenItem();
	virtual ~CUI_InvenItem();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

private:
	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	wstring m_Item_Panel;
};

