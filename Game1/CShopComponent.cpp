#include "stdafx.h"
#include "CShopComponent.h"

#include "CItem.h"
#include "CItem_Factory.h"

CShopComponent::CShopComponent()
{

}

CShopComponent::~CShopComponent()
{

}

void CShopComponent::BeginPlay()
{
	CComponent::BeginPlay();


}

void CShopComponent::TickComponent()
{
	CComponent::TickComponent();

	for (auto iter = m_Items.begin(); iter != m_Items.end(); )
	{
		auto item = (*iter).second;

		if (item->IsDestroy()) 
		{
			iter = m_Items.erase(iter);
			continue;
		}

		item->Tick();
		RENDER->AddRenderObject(item.get());

		++iter;
	}
}

void CShopComponent::Render()
{
	CComponent::Render();

	for (auto& item : m_Items)
	{
		if (!item.second->IsDestroy())
			item.second->Render();
	}

}

void CShopComponent::Release()
{
	CComponent::Release();

}

void CShopComponent::Equip_Item(EItemType Type)
{
	auto it = m_Items.find(Type);
	if (it == m_Items.end()) 
	{ 
		string id = "";
		switch (Type)
		{
		case EItemType::Consumable:
			id = "Heal";
			break;

		case EItemType::Weapon:
			id = "Weapon1";
			break;

		case EItemType::Armor:
			id = "Armor1";
			break;
		}

		Equip_ItemById(id);
	}
}

void CShopComponent::Equip_ItemById(const string& id)
{
	const FItemData* data = ITEM_DATA->GetById(id);
	if (!data) return;

	auto& slot = m_Items[data->ItemType];

	if (!slot)
	{
		slot = CItem_Factory::CreateItemID(id);

		if (slot)
			slot->BeginPlay();
	}

	else
	{
		// TODO : 이미 같은 타입이 있으면 추가 처리?
	}
}
