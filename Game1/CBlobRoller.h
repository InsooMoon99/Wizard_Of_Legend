#pragma once

#include "CMonsterBase.h"

class CBlobRoller : public CMonsterBase
{
public:
	CBlobRoller();
	virtual ~CBlobRoller();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void State_Idle();
	void State_Attack();
	void State_Hit();

public:
	// Interface
	void Update_Hit() override;
	void Hit_Color() override;
	void Hit_Effect() override;

protected:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;
	void Resource_Settings() override;

private:


};

