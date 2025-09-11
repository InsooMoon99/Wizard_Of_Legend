#pragma once

#include "CCharacter.h"

class CPlayer : public CCharacter
{
public:
	CPlayer();
	virtual ~CPlayer() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

public:


public:
	ObRect* testRect;

};

