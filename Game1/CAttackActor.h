#pragma once

#include "CSkill_Actor.h"

class CAttackActor : public CSkill_Actor
{
public:
	CAttackActor(FSkillInfo& _Info, const Vec2& Pos);
	virtual ~CAttackActor();

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
	set<CActor*> m_HitPlayer;

	bool bCheck = false;

	float m_Lifetime = 0.f;

};

