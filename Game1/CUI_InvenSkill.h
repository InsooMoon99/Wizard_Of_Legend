#pragma once

#include "CUI.h"

class CUI_InvenSkill : public CUI
{
public:
	CUI_InvenSkill();
	virtual ~CUI_InvenSkill();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

private:
	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	vector<shared_ptr<ObImage>> m_Skill_Panel;
	vector<shared_ptr<ObImage>> m_Skill_Key;

	wstring m_Panel_Name;

};

