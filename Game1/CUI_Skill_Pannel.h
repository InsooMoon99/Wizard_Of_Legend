#pragma once

#include "CUI.h"

class CUI_Skill_Pannel : public CUI
{
public:
	CUI_Skill_Pannel();
	virtual ~CUI_Skill_Pannel();

public: 
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

private:
	vector<shared_ptr<ObImage>> m_Skill_UI;

	shared_ptr<ObImage> m_Pannel;

};

