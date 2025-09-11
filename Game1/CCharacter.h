#pragma once

#include "CActor.h"

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;


public:
	void  SetHp(float _Hp) { m_Hp = _Hp; }

public:
	float GetHp()		  { return m_Hp; }


private:
	float m_Hp = 100.f;
};

