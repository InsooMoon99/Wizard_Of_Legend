#pragma once

#include "CSkill_Actor.h"

class CFire_Dragon : public CSkill_Actor
{
	using Super = CSkill_Actor;

public:
	CFire_Dragon(FSkillInfo& _Info);
	virtual ~CFire_Dragon();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CheckHitWall() override;
	void OnHitWall(const Vec2& hitPos) override;

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

public:
	shared_ptr<ObImage> m_FireDragon;
	shared_ptr<ObImage> m_FireDragon_Tail;
	vector<shared_ptr<ObImage>> m_FireDragon_Particle;

	vector<shared_ptr<ObImage>> m_FireBall_Particle;

};