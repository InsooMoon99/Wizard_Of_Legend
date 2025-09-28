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

public:
	void				SetFiring(bool _Fire)		  { bFiring = _Fire; }
	bool				GetFiring()			  { return bFiring; }

	void				SetSlot(ESkillSlot _Slot) { m_Slot = _Slot; }
	ESkillSlot			GetSlot()		  { return m_Slot; }

	void				SetCoolDown(float _CoolDown) { m_DataCoolDown = _CoolDown; }
	float				GetCoolDown() { return m_DataCoolDown; }

	void				SetCoolRemain(float _CoolRemain) { m_CoolRemain = _CoolRemain; }
	float				GetCoolRemain() { return m_CoolRemain; }

	bool				IsCoolTime() const { return m_CoolRemain > 0.f; }

	shared_ptr<ObImage> GetIcon() { return m_Icon; }
	void				SetIcon(const wstring& path, const Vec2& size);

	FSkillInfo&			Get_Skill_Info()  { return m_Skill_Info; }
	const char*			GetSkill_Id() { return m_Id; }

	void				StartCoolDown() {m_CoolRemain = m_DataCoolDown;}

protected:
	const char* m_Id = "";
	FSkillInfo	m_Skill_Info;

	bool		bFiring = false;

	float		m_DataDamage;
	float		m_DataCoolDown = 0.f;
	float		m_CoolRemain = 0.f;

private:
	shared_ptr<ObImage> m_Icon;
	ESkillSlot m_Slot = ESkillSlot::Slot_END;

};

