#pragma once

#include <array>

#include "CUI.h"

class CWeaponComponent;

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

public:
	void SetWeapon(shared_ptr<CWeaponComponent> weapon) { m_Weapon = weapon; }

private:
	shared_ptr<ObRect>			m_PanelRect;
	vector<shared_ptr<ObImage>> m_Skill_Panel;
	vector<shared_ptr<ObImage>> m_Skill_Key;

	weak_ptr<CWeaponComponent>	m_Weapon;
	vector<FSkill_UiInfo>		m_Skill_Infos;
	vector<shared_ptr<ObRect>>	m_CoolTime_Rect;

	RECT m_WritePos[SKILL_PANEL];

	array<shared_ptr<ObImage>, SKILL_PANEL> m_IconClones;

};

