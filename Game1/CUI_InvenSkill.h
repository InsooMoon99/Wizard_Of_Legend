#pragma once

#include <array>

#include "CUI.h"

class CWeaponComponent;

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

	virtual void SetRender(bool _Render) override;

public:
	void SetWeapon(shared_ptr<CWeaponComponent> weapon) { m_Weapon = weapon; }
	void OnIntersectMouse();

	CDelegate<ESkillSlot>  FOnIntersectMouse;

private:
	wstring m_Panel_Name;

	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	vector<shared_ptr<ObImage>> m_Skill_Panel;
	vector<shared_ptr<ObImage>> m_Skill_Panel_Gold;
	vector<shared_ptr<ObImage>> m_Skill_Key;

	weak_ptr<CWeaponComponent> m_Weapon;
	vector<FSkill_UiInfo> m_Skill_Infos;

	array<shared_ptr<ObImage>, SKILL_PANEL> m_IconClones;

	bool bFirst = true;
	int	 m_LastHovered = -1;

};