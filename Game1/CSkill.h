#pragma once

#include "CFire_Ball.h"
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
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	virtual bool CanActivate(const SkillInfo& _Info) abstract;
	virtual void Activate(CActor* _Owner) abstract;

protected:
	SkillInfo& Get_Skill_Info() { return m_Skill_Info; }

public:
	float	Get_CoolDown() { return m_CoolDown; }
	void	Set_CoolDown(float _CoolDown) { m_CoolDown = _CoolDown; }

	bool GetFiring() { return bFiring; }
	void SetFiring(bool _Fire) { bFiring = _Fire; }

protected:
	SkillInfo m_Skill_Info;
	bool bFiring = false;

private:
	float m_CoolDown = 0.f;

	


};

