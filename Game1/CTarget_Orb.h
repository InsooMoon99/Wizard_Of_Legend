#pragma once

#include "CSkill_Actor.h"

struct FOrbState
{
	bool isThrow = false;
	bool isArrived = false;
	bool isHit = false;

	Vec2 direction = Vec2(0, 0); 
	Vec2 targetPos;

	bool  exploded = false; 
	bool  dead = false;

	float speed = 0.f;
	float boomTimer = 0.f;
};

enum class EShadowType
{
	Small,
	Big,

	END
};

class CTarget_Orb : public CSkill_Actor
{
public:
	CTarget_Orb(FSkillInfo& _Info);
	virtual ~CTarget_Orb();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CheckHitWall();
	void OnHitWall(const Vec2& hitPos);

	void OnThrowOrb(int value);

	void DestroyOrb(int idx);

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

	void OnHitBySpear(GameObject* collider);
	void RecreateOrb(int i);

private:
	class CBoss_Controller* m_BossController;

	vector<shared_ptr<ObCircle>>	m_Target_Orb;
	vector<shared_ptr<ObImage>>		m_Target_Img;
	vector<shared_ptr<ObImage>>		m_OrbShadows;

	vector<FOrbState> m_OrbStates;

	float m_Angle[TARGET_ORB] = { 0.f };
	float m_Radius = 100.f;

	Vec2 m_OrbSize = Vec2(20.f, 11.f) * 2.8f;

	set<GameObject*> m_HitObjects;

	float m_boomTimer = 0.3f;
};

