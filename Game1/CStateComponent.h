#pragma once

#include "CComponent.h"
#include "CDelegate.h"

class CStateComponent : public CComponent
{
	using Super = CComponent;

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

	CDelegate<ECharacterState, EPlayerDir> OnBossStateChanged;

private:
	ECharacterState m_PlayerState;
	ECharacterState m_LastAttackType;

	EBossState		m_BossState;

	EPlayerDir		m_PlayerDir;


};


