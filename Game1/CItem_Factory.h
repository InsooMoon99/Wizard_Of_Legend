#pragma once

class CItem;

class CItem_Factory
{
public:
	CItem_Factory() {};
	~CItem_Factory() {};

public:
	static shared_ptr<CItem> CreateItemType(EItemType _ItemType);
	static shared_ptr<CItem> CreateItemID(const string& id);
};

