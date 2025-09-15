#pragma once
#include "CComponent.h"

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
	void Resource_Settings();

private:
	vector<shared_ptr<ObImage>> m_HUD;

};

