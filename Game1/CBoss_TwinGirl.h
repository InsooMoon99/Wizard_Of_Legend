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

	virtual void OnStateChange(ECharacterState _State, EPlayerDir _Dir) override;

public:
	void State_Start();
	void State_Start2();
	void State_Ready();

	void State_Air();

	void State_Idle();
	void State_Walk();
	void State_Attack1();
	void State_Attack2();
	void State_Attack3();

	void OnBossStart();
	void OnBossReady();
	void OnDashStart(const Vec2& _TargetPos);

	void OnJavelin(const Vec2& _Pos);
	void OnStarPos(const Vec2& _Pos);

	Vec2 GetJavelinPos() { return m_JavelinPos; }

protected:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;

	void Resource_Settings() override;

private:
	bool bStartPlay = false;
	bool bDashCheck = false;
	bool bFirst = false;

	float m_waitTime = 1.f;
	float m_UpTimer = 0.3f;

	shared_ptr<ObImage> m_Teleport;
	shared_ptr<ObImage> m_Teleport_Floor;
	shared_ptr<ObImage> m_Dash_Left;
	shared_ptr<ObImage> m_Dash_Right;
	shared_ptr<ObImage> m_Jump;

	shared_ptr<ObImage> m_JavelinSpawn;
	shared_ptr<ObImage> m_JavelinThrow;

	shared_ptr<ObImage> m_StarStart;

	Vec2 m_DashDir;

	float m_ShakeTimer = 0.f; 
	float m_OriginalY = 0.f;

	Vec2 m_JavelinPos{};
	bool bJavelinCheck = false;
	int  m_JavelinCount = 0;

	Vec2 m_StarPos {};
};

