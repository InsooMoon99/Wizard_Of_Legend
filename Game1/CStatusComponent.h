#pragma once

#include "CComponent.h"

class CStatusComponent : public CComponent
{
public:
	CStatusComponent();
	~CStatusComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	void	SetHp(float _Hp) { m_Hp = _Hp; }
	void	SetMp(float _Mp) { m_Mp = _Mp; }
	void	SetSpeed(float _Speed) { m_Speed = _Speed; }

public:
	float GetHp() { return m_Hp; }
	float GetMp() { return m_Mp; }
	float GetSpeed() { return m_Speed; }

private:

	float m_Hp;
	float m_Mp;
	float m_Speed;
};

