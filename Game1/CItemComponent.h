#pragma once

#include "CComponent.h"

class CItemComponent : public CComponent
{
public:
	CItemComponent();
	~CItemComponent() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

private:


};


