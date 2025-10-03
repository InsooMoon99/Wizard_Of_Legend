#pragma once

#include "CSkill_Actor.h"

class CBoss_Controller;
class CBossPawn;
class CBoss_TwinGirl;

class CJavelin_Spear : public CSkill_Actor
{
public:
	CJavelin_Spear(FSkillInfo& _Info);
	virtual ~CJavelin_Spear();

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
	bool bReleaseCheck = false;
	float m_ReleaseTime = 0.3f;

	float m_LifeTime = 0.f;

	CBoss_Controller* m_BossController;
	CBoss_TwinGirl* m_Boss;

	shared_ptr<ObImage> m_Spear;
	Vec2 m_AttackPos{ };

	set<GameObject*> m_HitObjects;
};

