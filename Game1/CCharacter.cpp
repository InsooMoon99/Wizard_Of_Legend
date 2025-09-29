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
	//m_Status->SetHp(m_Status->GetHp() - _Hit.Damage);
	//m_State->Change_State(ECharacterState::Hit);
}

void CCharacter::UpdateKnockbackTimer()
{
	if (!m_Knock.active) return;

	m_Knock.t += DELTA;

	if (m_Knock.t >= m_Knock.duration)
	{
		m_Knock.active = false;
	}
}

void CCharacter::SetDead()
{
	m_Status->SetHp(0.f);
	m_State->Change_State(ECharacterState::Dead);

	// 스프라이트 재생 끝으로 미루기
	//m_PendingKill = true;
}

