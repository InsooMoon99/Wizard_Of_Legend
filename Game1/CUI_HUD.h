#pragma once

#include "CUI.h"

class CUI_HUD : public CUI
{
public:
	CUI_HUD();
	virtual ~CUI_HUD();

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

public:
	void		AddChild(shared_ptr<CUI> _Ui);
	bool		RemoveChild(shared_ptr<CUI> _Ui);

protected:
	vector<shared_ptr<CUI>> m_Children;
};

