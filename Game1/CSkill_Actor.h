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

	void SetPendingKill(bool _Pending) { m_PendingKill = _Pending; }
	bool GetPendingKill() { return m_PendingKill; }

public:
	bool IsDestroy() const { return m_PendingKill; }

protected:
	FSkillInfo m_Skill_Info;
	Vec2	m_StartPos;

	float   m_SpawnLife = 0.f;
	bool	m_PendingKill = false;

};


