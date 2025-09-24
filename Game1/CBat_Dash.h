#pragma once

#include "CSkill_Actor.h"

class CBat_Dash : public CSkill_Actor
{
public:
	CBat_Dash(FSkillInfo& _Info);
	virtual ~CBat_Dash();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CheckHitWall();
	void OnHitWall(const Vec2& hitPos);

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

private:
	shared_ptr<ObImage> m_DashEffect;
	shared_ptr<ObImage> m_DashEffect2;

	class CBossPawn* m_Pawn;
	Vec2  m_DashDir;
	float m_TargetDistance = 0.f;
};

