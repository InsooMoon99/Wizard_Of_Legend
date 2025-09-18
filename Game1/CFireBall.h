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
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

	bool IsDestroy() const {return m_PendingKill;}

public:
	shared_ptr<ObCircle> m_FireBall_Col;
	shared_ptr<ObImage> m_FireBall;

	SkillInfo m_Skill_Info;
	Vec2	m_StartPos;

	float   m_SpawnLife = 0.f;

	bool	m_PendingKill = false;
};

