#pragma once

#include "CActor.h"

class CFireBall : public CActor
{
public:
	CFireBall(SkillInfo& _Info);
	virtual ~CFireBall() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	shared_ptr<ObCircle> m_FireBall_Col;
	shared_ptr<ObImage> m_FireBall;

	Vec2	m_MoveDir;
	float	m_Damage = 0.f;

};

