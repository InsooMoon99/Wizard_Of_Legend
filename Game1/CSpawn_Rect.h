#pragma once

#include "CSkill_Actor.h"

class CSpawn_Rect : public CSkill_Actor
{
public:
	CSpawn_Rect(FSkillInfo& _Info);
	virtual ~CSpawn_Rect();
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

private:
	class CBoss_Controller* m_BossController;
};

