#pragma once

#include "CUI.h"

class CUI_Inventory : public CUI
{
public:
	CUI_Inventory();
	virtual ~CUI_Inventory();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:


private:
	vector<shared_ptr<ObImage>> m_Inventory;

	//shared_ptr<ObImage> m_Inventory;

};

