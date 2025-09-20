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
	void Change_State(ECharacterState _State);
	void Change_Dir(EPlayerDir _Dir);

	ECharacterState Set_AttackType();
	void Set_AttackType(ECharacterState _State) { m_LastAttackType = _State; }

public:
	ECharacterState Get_State()		{ return m_PlayerState; }

	EPlayerDir	 Get_Dir()			{ return m_PlayerDir; }

private:
	ECharacterState m_PlayerState;
	ECharacterState m_LastAttackType;

	EPlayerDir	 m_PlayerDir;

};


