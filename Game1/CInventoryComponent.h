#pragma once

#include "CComponent.h"

class CItem;

class CInventoryComponent : public CComponent
{
public:
	CInventoryComponent();
	~CInventoryComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	bool AddItem(shared_ptr<CItem> item);
	void RemoveItem(CItem* item);

	const vector<shared_ptr<CItem>>& GetItems() { return m_Items; }
	vector<FItemUIInfo> GetItem_UiInfo(int maxCount = 5) const;

private:
	vector<shared_ptr<CItem>> m_Items;

};

