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
	virtual bool CanActivate(const FSkillInfo& _Info) abstract;
	virtual void Activate(CActor* _Owner) abstract;

	void SetIcon(const wstring& path, const Vec2& size);

protected:
	FSkillInfo& Get_Skill_Info() { return m_Skill_Info; }

	shared_ptr<ObImage> m_Icon;

public:
	void	Set_CoolDown(float _CoolDown) { m_CoolDown = _CoolDown; }
	void	SetFiring(bool _Fire)		  { bFiring = _Fire; }

	float	Get_CoolDown()		  { return m_CoolDown; }
	bool	GetFiring()			  { return bFiring; }

protected:
	FSkillInfo m_Skill_Info;
	bool bFiring = false;

private:
	float m_CoolDown = 0.f;

	


};

