#pragma once

#include "CObject.h"
#include "CActor.h"

class CItem : public CActor
{
public:
	CItem();
	~CItem() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void LateUpdate() override;
	void Release() override;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

protected:
	// Json으로 데이터 가져와서 세팅해주기 <- 집가서 ㄱ
	FItemData m_ItemData;

};


