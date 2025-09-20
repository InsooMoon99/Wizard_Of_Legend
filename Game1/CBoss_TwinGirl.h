#pragma once

#include "CBossPawn.h"

class CWeaponComponent;
class CUIComponent;
class CBoss_Controller;

class CBoss_TwinGirl : public CBossPawn
{
	using Super = CBossPawn;

public:
	CBoss_TwinGirl(CBoss_Controller* _Controller);
	virtual ~CBoss_TwinGirl();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void Update_Hit() override;
	void Hit_Color() override;
	void Hit_Effect() override;

private:
	// Components
	shared_ptr<CWeaponComponent> m_Weapon;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

public:
	void State_Idle();
	void State_Walk();
	void State_Attack();

protected:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;

	void Resource_Settings() override;

private:

};

