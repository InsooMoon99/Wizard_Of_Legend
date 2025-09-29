#include "stdafx.h"
#include "CItem_Factory.h"

#include "CItem_Potion.h"
#include "CItem_Weapon.h"
#include "CItem_Armor.h"

shared_ptr<CItem> CItem_Factory::CreateItemType(EItemType _ItemType)
{
	switch (_ItemType)
	{
	case EItemType::Consumable:
		return make_shared <CItem_Potion>();

	case EItemType::Weapon:

		break;

	case EItemType::Armor:

		break;

	case EItemType::Etc:

		break;
	}
}

shared_ptr<CItem> CItem_Factory::CreateItemID(const string& id)
{
	const FItemData* data = ITEM_DATA->GetById(id);

	if (!data) return nullptr;

	shared_ptr<CItem> item = nullptr;
	switch (data->ItemType)
	{
	case EItemType::Consumable: 
		item = make_shared<CItem_Potion>();
		break;
	case EItemType::Weapon:
		item = make_shared<CItem_Weapon>();
		break;
	case EItemType::Armor:
		item = make_shared<CItem_Armor>();
		break;
	case EItemType::Etc:

		break;
	}

	if (item) 
		item->SetData(*data);

	return item;
}
