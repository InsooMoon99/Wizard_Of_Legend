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
	void	SetMaxHp(float _Hp) { m_MaxHp = _Hp; }
	void	SetMaxMp(float _Mp) { m_MaxMp = _Mp; }
	void	SetHp(float _Hp) { m_Hp = _Hp; }
	void	SetMp(float _Mp) { m_Mp = _Mp; }
	void	SetSpeed(float _Speed) { m_Speed = _Speed; }
	void	SetGold(int _Gold) { m_Gold = _Gold; }

public:
	float GetMaxHp() { return m_MaxHp; }
	float GetMaxMp() {return m_MaxMp;}
	float GetHp() { return m_Hp; }
	float GetMp() { return m_Mp; }
	float GetSpeed() { return m_Speed; }
	int   GetGold() { return m_Gold; }

private:
	float m_MaxHp = 0.f;
	float m_MaxMp = 0.f;
	float m_Hp = 0.f;
	float m_Mp = 0.f;
	float m_Speed = 0.f;
	int   m_Gold = 0;
};

