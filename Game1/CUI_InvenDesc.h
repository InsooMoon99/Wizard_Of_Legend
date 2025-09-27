#pragma once

#include "CUI.h"

class CWeaponComponent;

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

public:
	void OnMouseIntersect(ESkillSlot index);
	void SetWeapon(shared_ptr<CWeaponComponent> Weapon) { m_Weapon = Weapon; }

private:
	shared_ptr<CWeaponComponent> m_Weapon;

	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	wstring m_Panel;

	ESkillSlot m_SelectSlot = LButton;
	vector<FSkill_UiInfo> m_UiInfos;

	RECT m_NameRect;
	RECT m_DescRect;
};

