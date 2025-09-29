#pragma once

#include "CUI.h"

struct FSlot
{
	shared_ptr<ObImage> bg;
	shared_ptr<ObImage> icon;
	string currentId;
};

class CInventoryComponent;

class CUI_InvenItem : public CUI
{
public:
	CUI_InvenItem();
	virtual ~CUI_InvenItem();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void SetInven(shared_ptr<CInventoryComponent> inven) { m_Inven = inven; }
	void OnIntersectMouse();

	CDelegate<int>  FOnItemIntersectMouse;

private:
	weak_ptr<CInventoryComponent> m_Inven;

	shared_ptr<ObImage> m_OutRect;
	shared_ptr<ObImage> m_InRect;

	wstring m_Item_Panel;

	vector<FSlot> m_PanelSlots;
	vector<shared_ptr<ObImage>> m_Panel_Gold;

	int   m_Cols = 5;
	float m_SpacingX = 62.f; 
	Vec2  m_GridStart = Vec2(-330.f, -154.f);

	bool bFirst = true;
	int	 m_LastHovered = -1;
};

