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
	// 그 전에 쿨타임 먼저 진행하기

};


