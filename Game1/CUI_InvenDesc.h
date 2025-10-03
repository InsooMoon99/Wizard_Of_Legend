#pragma once

#include "CUI.h"

class CWeaponComponent;
class CInventoryComponent;

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
	void OnSkillMouseIntersect(ESkillSlot index);
	void OnItemMouseIntersect(int index);

	void SetWeapon(shared_ptr<CWeaponComponent> Weapon) { m_Weapon = Weapon; }
	void SetInventory(shared_ptr<CInventoryComponent> Inven) { m_Inven = Inven; }

	void RenderSkillDesc();
	void RenderItemDesc();

private:
	weak_ptr<CWeaponComponent>		m_Weapon;
	weak_ptr<CInventoryComponent>	m_Inven;

	shared_ptr<ObImage>				m_OutRect;
	shared_ptr<ObImage>				m_InRect;

	wstring m_Panel;

	ESkillSlot m_SelectSlot = LButton;
	int m_SelectItemSlot = 0;

	vector<FSkill_UiInfo>			m_Skill_Infos;
	vector<FItemUIInfo>				m_Item_Infos;

	RECT m_NameRect;
	RECT m_DescRect;

	EDescMode m_DescMode;
};

