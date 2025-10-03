#pragma once

#include "CSkill_Actor.h"

class CStar_Collision : public CSkill_Actor
{
public:
	CStar_Collision(FSkillInfo& _Info, EStarType _Type);
	virtual ~CStar_Collision();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

private:
	shared_ptr<ObImage> m_Neddle;

	set<CActor*> m_HitPlayer;

	bool bCheck = false;

};

