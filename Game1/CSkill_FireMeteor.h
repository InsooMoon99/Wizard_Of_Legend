#pragma once

#include "CFire_Ball.h"
#include "CSkill.h"

class CActor;
class CFire_Meteor;

class CSkill_FireMeteor : public CSkill
{
public:
	CSkill_FireMeteor();
	virtual ~CSkill_FireMeteor();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CanActivate(const FSkillInfo& _Info) override;
	void Activate(CActor* _Owner) override;

private:
	void SpawnOneMiddleMeteor(int index);

private:
	vector<shared_ptr<CSkill_Actor>> m_FireMeteor;
	vector<shared_ptr<CSkill_Actor>> m_FireMiddleMeteor;
	shared_ptr<ObImage> m_CastingCircle;

	Vec2 m_PointPos {};

	bool bMiddleCheck = false;
	bool bMeteorCheck = false;

	const float angleStep = (2.0f * PI) / 5;

	int   m_MiddleCount = 0;    
	int   m_MiddleMax = 8;
	float m_SpawnTimer = 0.f;   
	float m_NextSpawnTime = 0.f;   
	bool  bMiddleSpawning = false; 
};

