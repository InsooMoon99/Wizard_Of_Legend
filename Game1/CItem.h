#pragma once

#include "CComponent.h"

class CItem : public CComponent
{
public:
	CItem();
	~CItem() override;

public:
	void BeginPlay() override;
	void TickComponent() override;
	void Render() override;
	void Release() override;

private:


};


