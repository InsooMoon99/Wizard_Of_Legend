#pragma once

#include "CItem.h"

class CItem_Potion : public CItem, public enable_shared_from_this<CItem>
{
public:
	CItem_Potion();
	~CItem_Potion() override;

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


};

