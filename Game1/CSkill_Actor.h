#pragma once

#include "CActor.h"

class CSkill_Actor : public CActor
{
public:
	CSkill_Actor(FSkillInfo& _Info);
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
	virtual bool CheckHitWall();
	virtual void OnHitWall(const Vec2& hitPos);

protected:
	FSkillInfo m_Skill_Info;
	Vec2	m_StartPos;

	float   m_SpawnLife = 0.f;

};


