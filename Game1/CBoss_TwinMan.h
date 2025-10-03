#pragma once

#include "CBossPawn.h"

class CWeaponComponent;
class CUIComponent;
class CBoss_Controller;

class CBoss_TwinMan : public CBossPawn
{
	using Super = CBossPawn;

public:
	CBoss_TwinMan(CBoss_Controller* _Controller);
	virtual ~CBoss_TwinMan();

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

	virtual void OnStateChange(ECharacterState _State, EPlayerDir _Dir) override;

public:
	void State_Wait();
	void State_Start();
	void State_Start2();
	void State_Ready();

	void State_Idle();
	void State_Walk();
	void State_Attack1_Ready();
	void State_Attack1();
	void State_Attack2();
	void State_Attack3();

	void State_Dead();

	void TakeDamage(FHitDamage _Hit) override;

protected:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;

	void Resource_Settings() override;


private:
	bool bAnimStarted = false;
	bool bAttackCheck = false;

	int  m_AttackCount = 0;
	bool bCountBat = false;

	bool bAttack2 = false;
	int  m_Attack2Count = 0;

	float m_Timer = 0.5f;

	shared_ptr<ObImage> m_Bat_Spawn;
	shared_ptr<ObImage> m_Bat_Windup;
	shared_ptr<ObImage> m_Bat_Throw;

	shared_ptr<ObImage> m_MetalOrb_Man;

	shared_ptr<ObImage> m_Target_Spawn;
	shared_ptr<ObImage> m_Target_Throw;

	shared_ptr<ObImage> m_FloorCrater;

	shared_ptr<ObImage> m_Shadow;
};

