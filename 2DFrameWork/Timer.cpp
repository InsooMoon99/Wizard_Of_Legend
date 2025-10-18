#include "framework.h"      
#include "Timer.h"

#include <thread>           
#include <algorithm>        

Timer::Timer()
{
    lastTime = chrono::steady_clock::now();
    deltaTime = 0.0f;
    worldTime = 0.0f;

    framePerSecond = 0;
    framePerSecondCount = 0;
    framePerSecondTimeElapsed = 0.0f;

    // 스케일 상태
    timeScale = 1.0f;
    targetScale = 1.0f;
    easeInDur = easeInLeft = 0.f;
    holdLeft = 0.f;
    easeOutDur = easeOutLeft = 0.f;

    m_bt = {};
}

bool Timer::GetTick(float& time, float interval)
{
    time += GetDeltaTime(); 
    if (time > interval)
    {
	    time -= interval; return true;
    }
    return false;
}

bool Timer::GetTick(float& t, float interval, bool scaled)
{
    t += (scaled ? GetDeltaTime() : GetDelta());
    if (t > interval) { t -= interval; return true; }
    return false;
}

void Timer::SetTimeScale(float scale, float easeIn, float easeOut)
{
    targetScale = max(0.f, scale);
    easeInDur = easeInLeft = max(0.f, easeIn);
    easeOutDur = easeOutLeft = max(0.f, easeOut);
}

void Timer::HitStop(float scale, float hold, float easeOut)
{
    timeScale = max(0.f, scale);
    targetScale = 1.0f; // 복귀 목표는 항상 1.0
    holdLeft = max(0.f, hold);
    easeOutDur = easeOutLeft = max(0.f, easeOut);
}

void Timer::QueueBulletTime(float scale, float easeIn, float hold, float easeOut, float delay)
{
    m_bt.queued = true;
    m_bt.delay = max(0.f, delay);
    m_bt.active = false;
    m_bt.remain = max(0.f, hold);
    m_bt.scale = max(0.f, scale);
    m_bt.easeIn = max(0.f, easeIn);
    m_bt.easeOut = max(0.f, easeOut);
}

void Timer::HitStopThenBulletTime(float stopScale, float stopHold, float stopEaseOut,
    float bulletScale, float bulletEaseIn, float bulletHold, float bulletEaseOut)
{
    HitStop(stopScale, stopHold, stopEaseOut);
    QueueBulletTime(bulletScale, bulletEaseIn, bulletHold, bulletEaseOut,
        /*delay=*/max(0.f, stopHold + stopEaseOut));
}

void Timer::ResetTimeScale()
{
    timeScale = targetScale = 1.f;
    easeInLeft = easeInDur = 0.f;
    holdLeft = 0.f;
    easeOutLeft = easeOutDur = 0.f;
    m_bt = {};
}

void Timer::Chronometry(UINT lock)
{
    using namespace chrono;

    currentTime = steady_clock::now();
    duration<double> Delta = currentTime - lastTime;
    deltaTime = static_cast<float>(Delta.count());

    if (lock > 0) {
        const double minDt = 1.0 / static_cast<double>(lock);
        if (deltaTime < minDt) {
            auto sleepDur = std::chrono::duration<double>(minDt - deltaTime);
            std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::microseconds>(sleepDur));
            // 재측정
            currentTime = std::chrono::steady_clock::now();
            auto Delta = currentTime - lastTime;
            deltaTime = static_cast<float>(std::chrono::duration<double>(Delta).count());
        }
    }

    lastTime = currentTime;
    worldTime += deltaTime;

    framePerSecondTimeElapsed += deltaTime;
    framePerSecondCount++;
    if (framePerSecondTimeElapsed >= 1.0f)
    {
        framePerSecond = framePerSecondCount;
        framePerSecondCount = 0;
        framePerSecondTimeElapsed -= 1.0f;
    }

    if (easeInLeft > 0.f)
    {
        float step = min(deltaTime / max(1e-6f, easeInDur), 1.f);
        timeScale += (targetScale - timeScale) * step;
        easeInLeft -= deltaTime;
        if (easeInLeft <= 0.f) timeScale = targetScale;
    }

    if (holdLeft > 0.f)
    {
        holdLeft -= deltaTime;
    }

    if (easeOutLeft > 0.f && holdLeft <= 0.f && timeScale != targetScale)
    {
        float step = min(deltaTime / max(1e-6f, easeOutDur), 1.f);
        timeScale += (targetScale - timeScale) * step;
        easeOutLeft -= deltaTime;
        if (easeOutLeft <= 0.f) timeScale = targetScale;
    }

    // 5) 버렛타임 시퀀스(지연→진입→유지→복귀), 모두 Unscaled 기준
    if (m_bt.queued && !m_bt.active)
    {
        m_bt.delay -= deltaTime;
        if (m_bt.delay <= 0.f)
        {
            // 진입
            SetTimeScale(m_bt.scale, m_bt.easeIn, /*easeOut=*/0.f);
            m_bt.active = true;
            // remain은 유지 시간으로 계속 감소
        }
    }
    if (m_bt.active)
    {
        m_bt.remain -= deltaTime;
        if (m_bt.remain <= 0.f)
        {
            // 복귀
            SetTimeScale(1.0f, /*easeIn=*/0.f, m_bt.easeOut);
            m_bt.active = false;
            m_bt.queued = false;
        }
    }

    // 6) 안전 클램프
    timeScale = clamp(timeScale, 0.f, 2.0f);

    // 7) 최종 scaled delta 산출
    deltaScaleTime = deltaTime * timeScale;

    // (프로젝트가 app.deltaScale을 참조한다면 동기화)
    app.deltaScale = timeScale;
}
