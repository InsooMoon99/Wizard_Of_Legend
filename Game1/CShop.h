#pragma once

#include "CObject.h"

class CShop : public CObject
{
public:
	CShop();
	~CShop() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;
};

