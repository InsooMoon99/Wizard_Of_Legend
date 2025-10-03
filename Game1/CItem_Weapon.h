#pragma once

#include "CItem.h"

class CItem_Weapon : public CItem, public enable_shared_from_this<CItem>
{
public:
	CItem_Weapon();
	~CItem_Weapon() override;

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

