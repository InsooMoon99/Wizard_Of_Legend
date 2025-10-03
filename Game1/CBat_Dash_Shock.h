#pragma once

#include "CSkill_Actor.h"

class CBat_Dash_Shock : public CSkill_Actor
{
public:
	CBat_Dash_Shock(FSkillInfo& _Info);
	virtual ~CBat_Dash_Shock();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

private:
	shared_ptr<ObImage> m_Shock;
};

