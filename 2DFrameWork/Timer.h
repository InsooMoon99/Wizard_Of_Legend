#pragma once

#include <chrono>
#include <algorithm> 
#include <cstdint>

#ifndef UINT
typedef unsigned int UINT;
#endif

class Timer: public Singleton<Timer>
{
public:
    Timer();

    // 프레임 갱신 (lock: FPS 제한. 0이면 제한 없음)
    void   Chronometry(UINT lock = 0);

    // 델타 제공
    float  GetDelta()     const { return deltaTime; }       // Unscaled (UI/절대시간)
    float  GetDeltaTime() const { return deltaScaleTime; }  // Scaled  (월드/연출)

    // 누적 시간/FPS
    float  GetWorldTime() const { return worldTime; }
    int    GetFPS()       const { return framePerSecond; }

    // 일정 간격 틱 (기존 시그니처 유지: DELTA 무엇을 쓰는지 주의)
    bool   GetTick(float& time, float interval);                   // (월드용이라면 내부를 GetDeltaTime()로)
    bool   GetTick(float& time, float interval, bool scaled);      // scaled 선택형

    // 타임스케일 제어
    void   SetTimeScale(float scale, float easeIn = 0.f, float easeOut = 0.f);
    void   HitStop(float scale, float hold, float easeOut = 0.06f);

    // 버렛타임 예약/시퀀스(지연→진입→유지→복귀)
    void   QueueBulletTime(float scale, float easeIn, float hold, float easeOut, float delay = 0.f);
    void   HitStopThenBulletTime(float stopScale, float stopHold, float stopEaseOut,
        float bulletScale, float bulletEaseIn, float bulletHold, float bulletEaseOut);

    // 강제 복귀
    void   ResetTimeScale();

private:
    // 시간 소스
    std::chrono::steady_clock::time_point lastTime;
    std::chrono::steady_clock::time_point currentTime;

    // 델타(초)
    float deltaTime = 0.0f; // Unscaled
    float deltaScaleTime = 0.0f; // Scaled  (= deltaTime * timeScale)

    // 누적/통계
    float worldTime = 0.0f;
    int   framePerSecond = 0;
    int   framePerSecondCount = 0;
    float framePerSecondTimeElapsed = 0.0f;

    // 타임스케일(히트스톱/버렛타임용)
    float timeScale = 1.0f; // 현재 스케일
    float targetScale = 1.0f; // 목표 스케일

    // 이징/유지 타이머(항상 Unscaled로 갱신)
    float easeInDur = 0.f, easeInLeft = 0.f;
    float holdLeft = 0.f;
    float easeOutDur = 0.f, easeOutLeft = 0.f;

    // 버렛타임 시퀀스(지연 실행 → 유지 → 복귀)
    struct BulletTimeSeq
	{
        bool  queued = false; // 예약됨
        float delay = 0.f;   // 시작까지 지연 (Unscaled)
        bool  active = false; // 진행 중
        float remain = 0.f;   // 유지 시간 (Unscaled)
        float scale = 1.f;   // 목표 스케일 (예: 0.4)
        float easeIn = 0.f;   // 진입 이징
        float easeOut = 0.f;   // 복귀 이징
    } m_bt;
};
