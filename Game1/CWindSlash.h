#pragma once

#include "CSkill_Actor.h"

class CWindSlash : public CSkill_Actor
{
public:
	CWindSlash(FSkillInfo& _Info);
	virtual ~CWindSlash();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

private:


};
