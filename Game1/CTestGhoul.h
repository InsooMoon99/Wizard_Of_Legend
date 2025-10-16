#pragma once

#include "CMonsterBase.h"

class IMonsterAction;

class CTestGhoul : public CMonsterBase
{
public:
	CTestGhoul();
	virtual ~CTestGhoul();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
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

protected:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;
	void Resource_Settings() override;

	void MoveTile(const Vec2& dir, float speed);
	void Print_ImGui();

private:
	bool m_BehaviorCheck = false;
	bool m_OnLastFrameEdge = false;  
	int  m_LastFrameHitCount = 0;    

	float m_IdleTimer = 0.f;
	float m_Attack2Timer = 0.f;

	bool m_AttackCheck = false;

	shared_ptr<ObImage> m_Shadow;

private:
	unique_ptr<IMonsterAction> m_Action;
};

