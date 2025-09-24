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

void CCharacter::TakeDamage(FHitDamage _Hit)
{
	m_Status->SetHp(m_Status->GetHp() - _Hit.Damage);
	m_State->Change_State(ECharacterState::Hit);
}

