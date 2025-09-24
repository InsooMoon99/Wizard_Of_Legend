#pragma once

#include "CSkill_Actor.h"

class CBoss_Controller;

class CTarget_Orb : public CSkill_Actor
{
public:
    explicit CTarget_Orb(FSkillInfo& _Info);
    virtual ~CTarget_Orb();

public:
    void BeginPlay() override;
    void Tick() override;
    void LateUpdate() override;
    void Render() override;
    void Release() override;

public:
    bool CheckHitWall() override;
    void OnHitWall(const Vec2& hitPos) override;

    void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
    void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;


public:
    void ThrowTowards(const Vec2& worldTarget);
    void OnHitSpear();

    bool IsDestroy() const { return m_Destroy; }

private:
    CBoss_Controller* m_Controller;

    shared_ptr<ObImage>  m_OrbImage_Small;
    shared_ptr<ObImage>  m_OrbImage_Big;
    shared_ptr<ObImage>  m_OrbImage_Hit;

    shared_ptr<ObImage>  m_OrbShadow;

    Vec2  m_Direction = Vec2(1.f, 0.f);
    float m_Speed = 1500.f;
    bool  m_IsThrow = false;
    bool  m_Destroy = false;

    bool  m_prevDestroy = false;
    bool  m_CanHit = false;

    float m_boomTimer = 0.3f;

    // 수명 관리
    float m_Life = 5.0f;

    float m_StopRadius = 40.f;   
    float m_SlowRadius = 200.f; 
    float m_MinSpeed = 200.f;

    Vec2  m_Dest = Vec2(0.f, 0.f);
};
