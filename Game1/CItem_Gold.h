#pragma once

#include "CItem.h"

class CPlayer;

class CItem_Gold : public CItem, public enable_shared_from_this<CItem>
{
public:
	CItem_Gold();
	~CItem_Gold() override;

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

	bool m_Attracting = false;
	CPlayer* m_Target = nullptr;
	float m_Speed = 300.f;   
};

