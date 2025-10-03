#pragma once

#include "CUI_HUD.h"

class CUI_InvenSkill;
class CUI_InvenStatus;
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

	virtual void SetRendering(bool _Render) override;

public:
	shared_ptr<CUI_InvenCloth>  GetInvenCloth()	 { return m_Cloth; }
	shared_ptr<CUI_InvenSkill>  GetInvenSkill()	 { return m_Skill; }
	shared_ptr<CUI_InvenStatus> GetInvenStatus() { return m_Status; }
	shared_ptr<CUI_InvenItem>   GetInvenItem()	 { return m_Item; }
	shared_ptr<CUI_InvenDesc>   GetInvenDesc()	 { return m_Desc; }

private:
	shared_ptr<ObImage> m_Background;

	shared_ptr<CUI_InvenCloth>  m_Cloth;
	shared_ptr<CUI_InvenSkill>	m_Skill;
	shared_ptr<CUI_InvenStatus>	m_Status;
	shared_ptr<CUI_InvenItem>	m_Item;
	shared_ptr<CUI_InvenDesc>	m_Desc;
	
};

