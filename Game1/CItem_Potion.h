#pragma once

#include "CItem.h"

class CItem_Potion : public CItem
{
public:
	CItem_Potion();
	~CItem_Potion() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void LateUpdate() override;
	void Release() override;
};

