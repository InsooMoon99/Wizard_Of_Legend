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
	void SetController(CBoss_Controller* _Controller) { m_Controller = _Controller; }
	shared_ptr<CStatusComponent> GetStatus();

protected:
	void ReportHit(float _Damage);

private:
	CBoss_Controller* m_Controller = nullptr;
};

