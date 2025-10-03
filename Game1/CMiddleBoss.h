#pragma once

#include "CMonsterBase.h"

class CMiddleBoss : public CMonsterBase
{
public:
	CMiddleBoss();
	virtual ~CMiddleBoss();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void State_Spawn();
	void State_Idle();
	void State_Walk();
	void State_Attack1();
	void State_Attack2();
	void State_Hit();
	void State_Dead();

public:
	// Interface
	void Update_Hit() override;
	void Hit_Color() override;
	void Hit_Effect() override;

	virtual void TraceTarget() override;

	

	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

	void TakeDamage(FHitDamage _Hit) override;

	void DropItem() override;

protected:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;
	void Resource_Settings() override;

	void MoveTile(const Vec2& dir, float speed);

private:
	shared_ptr<CWeaponComponent> m_Weapon;
	shared_ptr<CUIComponent> m_UI;
	shared_ptr<ObImage> m_Shadow;
	shared_ptr<ObImage> m_AppearImg;

	float m_AppearTime = 0.f;

	bool m_BehaviorCheck = false;
	bool m_OnLastFrameEdge = false;  
	int  m_LastFrameHitCount = 0;    

	float m_IdleTimer = 0.f;
	float m_IdleChangeTimer = 2.5f;

	float m_Attack2Timer = 0.f;

	bool m_AttackCheck = false;

	bool  m_PendingA2 = false;
	float m_PendingA2Timer = 0.f;   
	float m_PendingA2Delay = 2.5f; 
};

