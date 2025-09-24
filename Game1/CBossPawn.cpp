#include "stdafx.h"
#include "CBossPawn.h"

#include "CBoss_Controller.h"

CBossPawn::CBossPawn(CBoss_Controller* _Controller)
{
	if (_Controller == nullptr)
		return;

	m_Controller = _Controller;
	_Controller->RegisterPawn(this);

	m_bKnockback = false;
}

CBossPawn::~CBossPawn()
{

}

void CBossPawn::BeginPlay()
{
	CCharacter::BeginPlay();

}

void CBossPawn::Tick()
{
	CCharacter::Tick();

}

void CBossPawn::LateUpdate()
{
	CCharacter::LateUpdate();

}

void CBossPawn::Render()
{
	CCharacter::Render();

}

void CBossPawn::Release()
{
	CCharacter::Release();

}

void CBossPawn::Update_Hit()
{
	CCharacter::Update_Hit();

}

void CBossPawn::Hit_Color()
{
	CCharacter::Hit_Color();

}

void CBossPawn::Hit_Effect()
{
	CCharacter::Hit_Effect();

}

void CBossPawn::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CCharacter::OnComponentBeginOverlap(collider, other);


}

void CBossPawn::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CCharacter::OnComponentEndOverlap(collider, other);

}

shared_ptr<CStatusComponent> CBossPawn::GetControlStatus()
{
	if (m_Controller)
		return m_Controller->GetStatusComponent();

	return nullptr;
}

shared_ptr<CStateComponent> CBossPawn::GetControlState()
{
	if (m_Controller)
		return m_Controller->GetStateComponent();

	return nullptr;
}

void CBossPawn::ReportHit(float _Damage)
{
	// 컨트롤러로 데미지 전달
	if (m_Controller)
		m_Controller->TakeDamage(_Damage);

}
