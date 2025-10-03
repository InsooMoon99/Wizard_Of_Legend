#pragma once

#include "CItem.h"

class CItem_Armor : public CItem, public enable_shared_from_this<CItem>
{
public:
	CItem_Armor();
	~CItem_Armor() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void LateUpdate() override;
	void Release() override;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

	void ActivateItem(CActor* actor) override;

private:
	shared_ptr<ObImage> m_PriceTag;
	wstring m_Price;
};

