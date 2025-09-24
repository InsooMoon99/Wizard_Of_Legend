#pragma once

#include "CSkill_Actor.h"
#include "CBat_Ball.h"

class CBat_Ball : public CSkill_Actor
{
public:
	CBat_Ball(FSkillInfo& _Info);
	virtual ~CBat_Ball();

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
	shared_ptr<ObImage> m_Batball;

	bool  m_Arrived = false;

	Vec2  m_Start, m_Target;
	Vec2  m_V0;              // 초기 속도
	float m_GravityY = -2000.f; // 좌표계에 맞춰 부호 조정
	float m_FlightTime = 0.7f;  // 도착까지의 시간
	float m_Timer = 0.f;        // 경과 시간

	// 중복 충돌이 왜 되는진 모르겠는데 일단 막기
	set<GameObject*> m_HitObjects; 
};

