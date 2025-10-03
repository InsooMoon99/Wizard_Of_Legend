#pragma once

#include "CCharacter.h"
#include "CMonsterBase.h"

class CDummy : public CMonsterBase
{
public:
	CDummy();
	virtual ~CDummy();

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

	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

	void TakeDamage(FHitDamage _Hit) override;

protected:
	void Resource_Settings() override;

private:
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	void Play_Sprite(ECharacterState _State) override;

private:
	shared_ptr<ObImage> Dummy_Image;


};

