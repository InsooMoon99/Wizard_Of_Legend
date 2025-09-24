#pragma once

#include "CCharacter.h"

class CBoss_Controller;

class CBossPawn : public CCharacter
{
public:
	CBossPawn(CBoss_Controller* _Controller);
	~CBossPawn() override;

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

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

public:
	CBoss_Controller* GetController() const { return m_Controller; }
	void SetController(CBoss_Controller* _Controller) { m_Controller = _Controller; }

	shared_ptr<CStatusComponent> GetControlStatus();
	shared_ptr<CStateComponent> GetControlState();

	virtual void OnStateChange(ECharacterState _State, EPlayerDir _Dir) {};

	void SetPlayer(CActor* _Player) { m_Player = _Player; }
	CActor* GetPlayer() { return m_Player; }

protected:
	void ReportHit(float _Damage);

protected:
	CBoss_Controller* m_Controller = nullptr;

	shared_ptr<ObImage> m_BossTaunt;

	CActor* m_Player;
};

