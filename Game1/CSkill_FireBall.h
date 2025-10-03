#pragma once

#include "CFire_Ball.h"
#include "CSkill.h"

class CActor;

class CSkill_FireBall : public CSkill
{
public:
	CSkill_FireBall();
	virtual ~CSkill_FireBall();

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
	vector<shared_ptr<CFire_Ball>> m_FireBall;

	bool  m_Bursting = false;   
	int   m_PendingShots = 0;       
	float m_ShotInterval = 0.216f;   
	float m_ShotTimer = 0.f;    
	bool  m_CooldownStarted = false;   

private:
	void FireOnce(); 

	bool  m_bulletTimePending = false;
	float m_btDelay = 0.f;  
	float m_btRemain = 0.f; 

};

