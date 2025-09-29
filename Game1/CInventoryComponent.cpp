#include "stdafx.h"
#include "CInventoryComponent.h"

#include "CItem.h"

CInventoryComponent::CInventoryComponent()
{
}

CInventoryComponent::~CInventoryComponent()
{
}

void CInventoryComponent::BeginPlay()
{
	CComponent::BeginPlay();
}

void CInventoryComponent::TickComponent()
{
	CComponent::TickComponent();

}

void CInventoryComponent::Render()
{
	CComponent::Render();
}

void CInventoryComponent::Release()
{
	CComponent::Release();
}

bool CInventoryComponent::AddItem(shared_ptr<CItem> item)
{
	if (!item) return false;


	m_Items.push_back(std::move(item));

	return true;
}

void CInventoryComponent::RemoveItem(CItem* item)
{
	m_Items.erase(remove_if(m_Items.begin(), m_Items.end(),
		[item](shared_ptr<CItem> ptr)
		{
			return ptr.get() == item;
		}), m_Items.end());
}

vector<FItemUIInfo> CInventoryComponent::GetItem_UiInfo(int maxCount) const
{
    std::vector<FItemUIInfo> out;
    out.reserve(maxCount);

    auto n = std::min<int>((int)m_Items.size(), maxCount);
    for (int i = 0; i < n; ++i)
    {
        const auto& it = m_Items[i];
        if (!it)
        {
	        out.push_back(FItemUIInfo{});
        	continue;
        }

        const auto& data = it->GetData(); 
        FItemUIInfo ui;
        ui.Id = data.Id;
        ui.Name = data.Name;  
        ui.Desc = data.Desc;
        ui.Value = (int)data.Value;
        ui.Type = data.ItemType;
        ui.IconPathW = ITEM_DATA->GetIconPathW(data.Id); 
        ui.IsEmpty = false;

        out.push_back(std::move(ui));
    }

    for (int i = n; i < maxCount; ++i)
        out.push_back(FItemUIInfo{}); // ºó½½·ÔÀ¸·Î ¹Ù·Î Ã¤¿ì±â

    return out;
}
