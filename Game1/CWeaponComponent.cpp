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
	// Weapon Component에서 구조체를 세팅하는 구조는 별로인 거 같다.
	// 이걸 그냥 Object 자체에서 세팅한 후에 Actor를 스폰하면서, 생성자에서 값을 넘겨주는게 더 깔끔할거 같음

	if (testFireBall)
	{
		testFireBall->Activate(GetOwner());
		testFireBall->BeginPlay();
	}
}
