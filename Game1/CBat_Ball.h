#pragma once
#include "CSkill_Actor.h"
#include <unordered_set>

class CBat_Ball : public CSkill_Actor
{
public:
    explicit CBat_Ball(FSkillInfo& info);
    ~CBat_Ball() override;

    void BeginPlay() override;
    void Tick() override;
    void LateUpdate() override;
    void Render() override;
    void Release() override;

    // 충돌 이벤트
    void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
    void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

protected:
    // 벽 충돌 (필요 시 사용)
    bool CheckHitWall() override;
    void OnHitWall(const Vec2& hitPos) override;

    // 안전한 강제 제거 (콜라이더 제거/비가시화 등)
    void KillNow();

private:
    // 상태
    bool  m_Arrived = false;   // 목표 지점 도달 여부
    float m_Timer = 0.f;     // 비행 경과 시간
    float m_LifeTime = 0.f;     // 생성 후 전체 경과 시간 (전역 상한 체크)
    float m_AfterArrive = 0.f;     // 도착 후 경과 시간 (유예 뒤 사망)

    // 포물선 파라미터
    Vec2  m_Start;                 // 시작 위치
    Vec2  m_Target;                // 목표 위치
    Vec2  m_V0;                    // 초기 속도

    // 튜닝 값
    float m_FlightTime = 0.45f;   // 비행 시간 (0 금지)
    float m_GravityY = -1600.f; // 중력가속도(엔진 좌표계에 맞춰 부호 조정)

    // 생존 상한/유예
    static constexpr float kMaxLife = 2.5f; // 생성 후 절대 수명
    static constexpr float kDespawnGrace = 0.6f; // 도착 후 유예(트윈걸 히트 대기)

    // 비주얼/충돌
    std::shared_ptr<ObCircle> m_Collision;
    std::shared_ptr<ObImage>  m_Batball;

    // 다중 히트 방지
    std::unordered_set<GameObject*> m_HitObjects;
};
