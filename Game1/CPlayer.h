#pragma once

#include "CCharacter.h"
#include "CEnums.h"
#include "CStruct.h"

class CComponent;
class CStateComponent;
class CWeaponComponent;
class CInventoryComponent;
class CUIComponent;
class CItem;

class CPlayer : public CCharacter
{
public:
	CPlayer();
	virtual ~CPlayer() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	// Interface
	void Update_Hit() override;
	void Hit_Color() override;
	void Hit_Effect() override;

private:
	// Components
	CWeaponComponent*		m_Weapon;
	CInventoryComponent*	m_Inventory;
	CUIComponent*			m_UI;

public:
	void Key_Input();

	void State_Idle();
	void State_Walk();
	void State_Dash();
	void State_Attack();

	virtual void SetSprite(EPlayerState _State, EPlayerDir _Dir) override;
	virtual void Play_Sprite(EPlayerState _State);

public:
	void Attack_Sprite();
	void Shadow_Size();
	void Dash_Sprite();

	//ObRect* Get_PlayerCollision() { return m_PlayerCol.get(); }
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

protected:
	// Player 한테 리소스 세팅을 할지?
	// ResourceManager로 옮길지 고민을 좀 해보기
	void Resource_Settings() override;

private:
	void Print_ImGui();
	void MoveTile(const Vec2& dir, float speed);

private:
	Vec2 m_MoveDir   =	{ 0, 0 };
	Vec2 m_LastDir   =	{ 0, -1.f };
	Vec2 m_AttackDir =	{ 0, 0 };

	float m_DashDistance = 1200.f;

	// Image
	unordered_map<EPlayerState, shared_ptr<ObImage>> m_Shadow;
	unordered_map<EPlayerDir, shared_ptr<ObImage>>   m_Dash;

	shared_ptr<ObImage> m_Player_Marker = nullptr;

private:
	Vec2	m_StartShadowScale;
	Vec2	m_EndShadowScale;

	float	m_DashTimer    = 0.4f;
	int		m_DashEndFrame = 11;
};

