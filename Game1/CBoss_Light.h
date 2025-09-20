#pragma once

#include "CCharacter.h"

class CWeaponComponent;
class CUIComponent;

class CBoss_Light : public CCharacter
{
public:
	CBoss_Light();
	virtual ~CBoss_Light();

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
	CWeaponComponent* m_Weapon;
	CUIComponent* m_UI;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

public:
	void State_Idle();
	void State_Walk();
	void State_Attack();

	void Set_ManPos(Vec2 _pos) { m_Twin_ManPos; }
	void Set_GirlPos(Vec2 _pos) { m_Twin_GirlPos; }

protected:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;

	void Resource_Settings() override;

private:
	shared_ptr<GameObject> m_GirCollision;

	Vec2 m_Twin_ManPos;
	Vec2 m_Twin_GirlPos;


};

