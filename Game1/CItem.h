#pragma once

#include "CObject.h"

class CItem : public CObject
{
public:
	CItem();
	~CItem() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;


private:


};


