#pragma once

#include "CComponent.h"

class CItem;

class CShopComponent : public CComponent
{
public:
	CShopComponent();
	~CShopComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	void Equip_ItemById(const string& id);

private:
	unordered_map<EItemType, shared_ptr<CItem>> m_Items;

};

