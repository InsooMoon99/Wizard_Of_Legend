#pragma once

#include "CSkill_Actor.h"

class CFire_MiddleMeteor : public CSkill_Actor
{
	using Super = CSkill_Actor;

public:
	CFire_MiddleMeteor(FSkillInfo& _Info);
	virtual ~CFire_MiddleMeteor();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CheckHitWall();
	void OnHitWall(const Vec2& hitPos);

	bool GetCood() { return bCood; }

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

private:
	shared_ptr<ObImage> m_FireMeteor;
	vector<shared_ptr<ObImage>> m_FireBall_Particle;

	float m_LifeTime = 0.f;

private:
	float m_RemainDist = 0.f;  
	float m_Speed = 700.f;

	bool bFirst = true;
	bool bCood = false;

	float m_ArmDist = 40.f; 
	bool  m_bArmed = false; 
	bool  m_bAddedToMgr = false; 
};

