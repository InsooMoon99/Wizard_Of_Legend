#pragma once

#include "CUI.h"

class CUI_InvenDesc : public CUI
{
public:
	CUI_InvenDesc();
	virtual ~CUI_InvenDesc();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

private:
	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	wstring m_Desc_Panel;

};

