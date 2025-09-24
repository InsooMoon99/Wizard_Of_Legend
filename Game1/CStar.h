#pragma once

#include "CSkill_Actor.h"

class CBoss_TwinGirl;
class CBoss_Controller;

class CStar : public CSkill_Actor
{
public:
	CStar(FSkillInfo& _Info);
	virtual ~CStar();

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
	CBoss_Controller* m_BossController;
	CBoss_TwinGirl* m_Boss;

	shared_ptr<ObImage> m_Star;
	Vec2 m_AttackPos{ };

};

