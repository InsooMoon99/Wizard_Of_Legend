#include "stdafx.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"

CCharacter::CCharacter()
{
	m_Status = CHelper::CreateSharedComponent<CStatusComponent>(this);
	m_State  = CHelper::CreateSharedComponent<CStateComponent>(this);

	m_prevState = ECharacterState::Idle;
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

