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
	shared_ptr<ObImage> m_FireDragonRight;
	shared_ptr<ObImage> m_FireDragonLeft;

	shared_ptr<ObImage> m_CurFireImg;

	shared_ptr<ObImage> m_FireDragon_Tail;
	vector<shared_ptr<ObImage>> m_FireDragon_Particle;

	vector<shared_ptr<ObImage>> m_FireBall_Particle;

	float m_SpawnLife = 0.f;

	float m_RandomSpeed;
	int   m_WaveMode = 0;   
	float m_WavePhase = 0.f;
	float m_LastSinOffset = 0.f; 
};