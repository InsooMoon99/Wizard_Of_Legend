#include "stdafx.h"
#include "CWeaponComponent.h"

#include "CSkill_FireBall.h"

CWeaponComponent::CWeaponComponent()
{

}

CWeaponComponent::~CWeaponComponent()
{
}

void CWeaponComponent::BeginPlay()
{
	CComponent::BeginPlay();

	testFireBall = make_shared<CSkill_FireBall>();
	testFireBall->BeginPlay();
}

void CWeaponComponent::TickComponent()
{
	CComponent::TickComponent();

	testFireBall->Tick();
}

void CWeaponComponent::Render()
{
	CComponent::Render();

	testFireBall->Render();
}

void CWeaponComponent::Release()
{
	CComponent::Release();
}

void CWeaponComponent::Fire_Ball()
{
	if (testFireBall)
	{
		testFireBall->Activate(GetOwner());
	}
}
