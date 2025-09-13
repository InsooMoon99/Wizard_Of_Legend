#pragma once

#include "CActor.h"

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter() override;

public:
	void BeginPlay() override abstract {}
	void Tick()		 override abstract {}
	void LateUpdate() override abstract {}
	void Render()	 override abstract {}
	void Release()	 override abstract {}

protected:
	virtual void Resource_Settings() abstract;

public:
	void  SetHp(float _Hp) { m_Hp = _Hp; }

public:
	float GetHp()		  { return m_Hp; }

protected:
	float m_Hp;
	float m_Speed;
};

