#include "stdafx.h"
#include "CStateComponent.h"

CStateComponent::CStateComponent()
{
	m_PlayerState = ECharacterState::Idle;
	m_PlayerDir = EPlayerDir::Front;

	m_LastAttackType = ECharacterState::Attack2;
}

CStateComponent::~CStateComponent()
{
}

void CStateComponent::BeginPlay()
{
	CComponent::BeginPlay();

}

void CStateComponent::TickComponent()
{
	CComponent::TickComponent();

}

void CStateComponent::Render()
{
	CComponent::Render();

}

void CStateComponent::Release()
{
	CComponent::Release();

}

void CStateComponent::Change_State(ECharacterState _State)
{
	if (m_PlayerState == _State)
		return;

	switch (_State)
	{
	case ECharacterState::Attack1:
	case ECharacterState::Attack2:
		m_LastAttackType = _State;
		break;
	}

	m_PlayerState = _State;
}

void CStateComponent::Change_Dir(EPlayerDir _Dir)
{
	if (m_PlayerDir == _Dir)
		return;

	m_PlayerDir = _Dir;
}

ECharacterState CStateComponent::Set_AttackType()
{
	ECharacterState next =(m_LastAttackType == ECharacterState::Attack1) ? ECharacterState::Attack2 : ECharacterState::Attack1;
	m_LastAttackType = next;

	return next;
}