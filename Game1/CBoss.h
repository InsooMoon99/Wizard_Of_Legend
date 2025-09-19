#pragma once

#include "CCharacter.h"

class CWeaponComponent;
class CUIComponent;

class CBoss : public CCharacter
{
public:
	CBoss();
	virtual ~CBoss();

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

protected:
	void SetSprite(EPlayerState _State, EPlayerDir _Dir) override;
	void Play_Sprite(EPlayerState _State) override;

	void Resource_Settings() override;


private:

};

