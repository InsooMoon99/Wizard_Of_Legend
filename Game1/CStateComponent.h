#pragma once

#include "CComponent.h"

class CStateComponent : public CComponent
{
public:
	CStateComponent();
	~CStateComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	void Change_State(EPlayerState _State);
	void Change_Dir(EPlayerDir _Dir);

	EPlayerState Next_AttackType();
	void Set_AttackType(EPlayerState _State) { m_LastAttackType = _State; }

public:
	EPlayerState Get_State()		{ return m_PlayerState; }

	EPlayerDir	 Get_Dir()			{ return m_PlayerDir; }

private:
	EPlayerState m_PlayerState;
	EPlayerState m_LastAttackType;

	EPlayerDir	 m_PlayerDir;

};


