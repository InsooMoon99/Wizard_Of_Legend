#pragma once

#include "CFireBall.h"
#include "CObject.h"
#include "CStruct.h"

class CSkill : public CObject
{
public:
	CSkill();
	virtual ~CSkill() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;

public:
	virtual bool CanActivate(const SkillInfo& _Info) abstract;
	virtual void Activate(CActor* _Owner);

public:
	float	Get_CoolDown() { return m_CoolDown; }
	void	Set_CoolDown(float _CoolDown) { m_CoolDown = _CoolDown; }

protected:
	float m_CoolDown = 0.f;

};

