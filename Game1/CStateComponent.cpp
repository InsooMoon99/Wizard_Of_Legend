#include "stdafx.h"
#include "CStateComponent.h"

CStateComponent::CStateComponent()
{
	m_PlayerState = EPlayerState::Idle;
	m_PlayerDir = EPlayerDir::Front;

	m_LastAttackType = EPlayerState::Attack2;
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

void CStateComponent::Change_State(EPlayerState _State)
{
	if (m_PlayerState == _State)
		return;

	switch (_State)
	{
	case EPlayerState::Attack1:
	case EPlayerState::Attack2:
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

EPlayerState CStateComponent::Set_AttackType()
{
	EPlayerState next =(m_LastAttackType == EPlayerState::Attack1) ? EPlayerState::Attack2 : EPlayerState::Attack1;
	m_LastAttackType = next;

	return next;
}

