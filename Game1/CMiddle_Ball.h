#pragma once
#include "CSkill_Actor.h"

class CMiddle_Ball : public CSkill_Actor
{
public:
    CMiddle_Ball(FSkillInfo& _Info);
    ~CMiddle_Ball();

public:
    void BeginPlay() override;
    void Tick() override;
    void LateUpdate() override;
    void Render() override;
    void Release() override;

    void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
    void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

    GameObject* Get_Collision() const { return m_Collision.get(); }

    void EnableOrbit(float radius, float startAngle, float angularSpeed, GameObject* parentRT)
    {
        m_Orbit = true;
        m_OrbitRadius = radius;
        m_OrbitAngle = startAngle;
        m_OrbitSpeed = angularSpeed;

        if (parentRT && m_Collision)
        {
            m_Collision->SetParentRT(*parentRT);
            Vec2 offset = Vec2(cosf(m_OrbitAngle), sinf(m_OrbitAngle)) * m_OrbitRadius;
            m_Collision->SetLocalPos(offset);
        }
    }

private:
    shared_ptr<ObCircle> m_Collision;
    shared_ptr<ObImage>  m_FireBall;

    Vec2  m_Dir = Vec2(1.f, 0.f);
    float m_LifeTime = 0.f;

    bool  m_Orbit = false;
    float m_OrbitRadius = 0.f;
    float m_OrbitAngle = 0.f;
    float m_OrbitSpeed = 0.f;   
};
