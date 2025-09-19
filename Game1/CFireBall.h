#pragma once

#include "CSkill_Actor.h"

class CFireBall : public CSkill_Actor
{
	using Super = CSkill_Actor;

public:
	CFireBall(SkillInfo& _Info);
	virtual ~CFireBall() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CheckHitWall();
	void OnHitWall(const Vec2& hitPos);

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

public:
	shared_ptr<ObImage> m_FireBall;

};

