#pragma once

#include "CUI.h"

class CUI_InvenCloth : public CUI
{
public:
	CUI_InvenCloth();
	virtual ~CUI_InvenCloth();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

private:
	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	shared_ptr<ObImage> m_ClothBack;
	shared_ptr<ObImage> m_Cloth;


	wstring m_Cloth_Panel;
};

