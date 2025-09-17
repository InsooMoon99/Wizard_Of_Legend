#pragma once

#include "CActor.h"

class CSkill_Actor : public CActor
{
public:
	CSkill_Actor();
	virtual ~CSkill_Actor();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

public:
};


