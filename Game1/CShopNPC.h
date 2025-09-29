#pragma once

#include "CCharacter.h"

class CShopComponent;
class CItem;

class CShopNPC : public CCharacter
{
public:
	CShopNPC();
	virtual ~CShopNPC();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

protected:
	void Resource_Settings() override;

public:
	void Update_Hit() override {}
	void Hit_Color() override  {}
	void Hit_Effect() override {}

private:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;

private:
	shared_ptr<ObImage> m_IdleImg;
	shared_ptr<ObImage> m_Mat;

	shared_ptr<CShopComponent>	m_Shop;

};

