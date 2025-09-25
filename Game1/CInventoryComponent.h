#pragma once

#include "CComponent.h"

class CInventoryComponent : public CComponent
{
public:
	CInventoryComponent();
	~CInventoryComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;


};

