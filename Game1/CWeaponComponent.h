#pragma once

#include "CComponent.h"

class CSkill;

class CWeaponComponent : public CComponent
{
public:
	CWeaponComponent();
	~CWeaponComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

public:
	void Fire_Ball();

private:
	shared_ptr<CSkill> testFireBall;
};

