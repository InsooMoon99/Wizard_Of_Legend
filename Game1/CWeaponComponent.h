#pragma once

#include "CComponent.h"

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

};

