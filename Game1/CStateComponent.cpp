#include "stdafx.h"
#include "CStateComponent.h"

CStateComponent::CStateComponent()
{
	m_PlayerState = EPlayerState::Idle;
	m_PlayerDir = EPlayerDir::Front;
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

	m_PlayerState = _State;
}

void CStateComponent::Change_Dir(EPlayerDir _Dir)
{
	if (m_PlayerDir == _Dir)
		return;

	m_PlayerDir = _Dir;
}
