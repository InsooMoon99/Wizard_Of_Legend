#pragma once

#include "CFireBall.h"
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
	bool CanActivate(const SkillInfo& _Info) override;
	void Activate(CActor* _Owner) override;

private:
	vector<shared_ptr<CFireBall>> m_FireBall;

};

