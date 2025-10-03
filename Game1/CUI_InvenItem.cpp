#include "stdafx.h"
#include "CUI_InvenItem.h"

#include "CInventoryComponent.h"
#include "CItem.h"

CUI_InvenItem::CUI_InvenItem()
{
	Vec2 outRectSize = Vec2(128.f * 1.1f, 56.f) * 3.f;
	m_OutRect = RESOURCE->CreateUI(L"UI\\Inventory_OutRect.png", outRectSize, true);
	m_OutRect->SetWorldPos(Vec2(-400.f, -105.f));
	m_OutRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

	Vec2 inRectSize = Vec2(131.f, 32.f) * 3.f;
	m_InRect = RESOURCE->CreateUI(L"UI\\Inventory_InRect.png", inRectSize, true);
	m_InRect->pivot = OFFSET_LT;
	m_InRect->SetParentT(*m_OutRect);
	m_InRect->SetLocalPos(Vec2(-197.f, 35.f));
	m_InRect->color = Color(0.5f, 0.5f, 0.5f, 0.45f);

    const int MAX_SLOTS = 5;
    m_PanelSlots.resize(MAX_SLOTS);
    m_Panel_Gold.resize(MAX_SLOTS);

    for (int i = 0; i < MAX_SLOTS; ++i)
    {
        m_PanelSlots[i].bg = RESOURCE->CreateUI(L"UI\\Panel.png", Vec2(52.f, 50.f), true);
        m_PanelSlots[i].bg->SetParentT(*m_InRect);

        int col = i; 
        Vec2 pos = m_GridStart + Vec2(col * m_SpacingX, 0);
    	m_PanelSlots[i].bg->SetLocalPos(pos);
        m_PanelSlots[i].bg->visible = true;
        m_PanelSlots[i].bg->collider = COLLIDER::RECT;

        m_PanelSlots[i].icon.reset();
    }

    for (int i = 0; i < MAX_SLOTS; i++)
    {
        int col = i;
        Vec2 pos = m_GridStart + Vec2(col * m_SpacingX, -1);

        m_Panel_Gold[i] = RESOURCE->CreateUI(L"UI\\GoldPanelUnfilled.png", Vec2(52.f, 50.f) * 1.1f, false);
        m_Panel_Gold[i]->SetParentT(*m_InRect);
        m_Panel_Gold[i]->SetLocalPos(pos);
    }

	m_Item_Panel = L"- 유물 - ";
}

CUI_InvenItem::~CUI_InvenItem()
{
	Release();
}

void CUI_InvenItem::BeginPlay()
{
	CUI::BeginPlay();

    FOnRendering.Add([this](bool bCheck)
        {
            this->SetRendering(bCheck);
        });

}

void CUI_InvenItem::Tick()
{
	CUI::Tick();

	m_OutRect->Update();
	m_InRect->Update();

    const vector<shared_ptr<CItem>>* items = nullptr;

    auto inven = m_Inven.lock();

    if (inven)
        items = &inven->GetItems();

    int count = items ? (int)items->size() : 0;
    int maxSlots = (int)m_PanelSlots.size();

    for (int i = 0; i < min(count, maxSlots); ++i)
    {
        auto& item = (*items)[i];
        auto& slot = m_PanelSlots[i];

        if (!item)
        {
            if (slot.icon)
                slot.icon->visible = false;

            slot.currentId.clear();

            continue;
        }

        const string& id = item->GetData().Id;
        Vec2 iconSize = item->GetIconSize();

        if (slot.currentId != id || !slot.icon)
        {
            wstring wkey = ITEM_DATA->GetIconPathW(id);

            slot.icon = RESOURCE->CreateUI(wkey, iconSize, true);
            slot.icon->SetParentT(*slot.bg);
            slot.icon->SetLocalPos(Vec2(-197.f, +35.f));   
            slot.currentId = id;
        }

        slot.icon->scale = iconSize;    
        slot.icon->visible = true;

        slot.bg->Update();
        if (slot.icon) slot.icon->Update();
    }

    for (int i = count; i < maxSlots; ++i)
    {
        m_PanelSlots[i].bg->Update();

        if (m_PanelSlots[i].icon)
        {
            m_PanelSlots[i].icon->visible = false;
            m_PanelSlots[i].icon->Update();
        }
            
    }

    for (auto& iter : m_Panel_Gold)
    {
        iter->Update();
    }

    OnIntersectMouse();

}

void CUI_InvenItem::LateUpdate()
{
	CUI::LateUpdate();

	
}

void CUI_InvenItem::Render()
{
	CUI::Render();

	DWRITE->RenderText(m_Item_Panel, RECT{ 208, 395,450,455 },
		18.0f, L"Noto Sans Cond Med", Color(1, 1, 1, 1));

	m_OutRect->Render();
	m_InRect->Render();

    for (auto& slot : m_PanelSlots)
    {
        if (slot.bg)
            slot.bg->Render();
    }
      
    for (auto& iter : m_Panel_Gold)
    {
        iter->Render();
    }

    for (auto& slot : m_PanelSlots)
    {
        if (slot.icon && slot.icon->visible)
            slot.icon->Render();
    }
}

void CUI_InvenItem::Release()
{
	CUI::Release();

}

void CUI_InvenItem::OnIntersectMouse()
{
    if (!GetRender()) return;

    int newHovered = -1;
    Vec2 mouse = INPUT->GetMouseScreenPos();

    for (int i = 0; i < 5; ++i)
    {
        auto panel = m_PanelSlots[i].bg;
        auto gold_panel = m_Panel_Gold[i];

        if (!panel || !gold_panel)
            continue;

        bool cood = panel->IntersectScreenMouse(mouse);

        if (cood)
        {
            gold_panel->visible = true;
            newHovered = i;
            break;
        }

        else
        {
            gold_panel->visible = false;
        }
    }

    if (newHovered != m_LastHovered)
    {
        if (newHovered != -1)
        {
            //SOUND->StopPlay("Select", 0.5f);
            FOnItemIntersectMouse.Broadcast(newHovered);
        }
        m_LastHovered = newHovered;
    }
}

