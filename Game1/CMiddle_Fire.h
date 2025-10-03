#pragma once

#include "CSkill_Actor.h"

class CMiddle_Fire : public CSkill_Actor
{
public:
	CMiddle_Fire(FSkillInfo& _Info);
	virtual ~CMiddle_Fire();

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
	shared_ptr<ObImage> m_FireBall;

	float m_LifeTime = 0.f;

	Vec2  m_Dir = Vec2(1, 0);
};
