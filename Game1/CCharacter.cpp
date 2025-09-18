#include "stdafx.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"

CCharacter::CCharacter()
{
	m_Status = CHelper::CreateComponent<CStatusComponent>(this);
	m_State  = CHelper::CreateComponent<CStateComponent>(this);

	m_prevState = EPlayerState::Idle;
	m_prevDir = EPlayerDir::Front;
}

CCharacter::~CCharacter()
{

}

void CCharacter::BeginPlay()
{
	CActor::BeginPlay();

}

void CCharacter::Tick()
{
	CActor::Tick();


}

void CCharacter::LateUpdate()
{
	CActor::LateUpdate();

}

void CCharacter::Render()
{
	CActor::Render();

}

void CCharacter::Release()
{
	CActor::Release();

}

