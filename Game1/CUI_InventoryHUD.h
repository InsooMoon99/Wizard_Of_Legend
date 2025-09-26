#pragma once

#include "CUI_HUD.h"

class CUI_InvenSkill;
class CUI_InvenItem;
class CUI_InvenDesc;
class CUI_InvenCloth;

class CUI_InventoryHUD : public CUI_HUD
{
public:
	CUI_InventoryHUD();
	virtual ~CUI_InventoryHUD();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

	virtual void SetRender(bool _Render) override;

public:
	shared_ptr<CUI_InvenSkill> GetInvenSkill() { return m_Skill; }

private:
	shared_ptr<ObImage> m_Background;

	shared_ptr<CUI_InvenSkill>	m_Skill;
	shared_ptr<CUI_InvenItem>	m_Item;
	shared_ptr<CUI_InvenDesc>	m_Desc;
	shared_ptr<CUI_InvenCloth>  m_Cloth;
	
};

