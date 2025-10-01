#include "framework.h"

static float SmoothStep(float x)
{
	return x * x * (3.f - 2.f * x);
}

Camera::Camera()
{
    //첫 카메라 위치
    position.x = 0.0f;
    position.y = 0.0f;
    //뷰포트
    viewport.width = app.GetWidth();
    viewport.height = app.GetHeight();
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
}

void Camera::Set()
{
    UpdateShake();

    Vector2 cam = position + m_ShakeOffset;   
    V = Matrix::CreateTranslation(-cam.x, -cam.y, 0.0f);
    P = Matrix::CreateOrthographic(app.GetWidth(), app.GetHeight(), 0.0f, 10.0f);

    VP = V * P;
}

void Camera::ResizeScreen()
{
    /*viewport.x = 200.0f;
    viewport.y = 200.0f;*/

    viewport.width = app.GetWidth();
    viewport.height = app.GetHeight();
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
}

void Camera::StartShake(float amplitude, float frequency, float duration, float falloff)
{
    m_Amplitude = max(0.f, amplitude);
    m_Frequency = max(0.01f, frequency);
    m_ShakeDuration = max(0.f, duration);
    m_Falloff = max(0.5f, falloff);
    m_ShakeTime = 0.f;
    m_Trauma = max(m_Trauma, 0.35f); // 즉발감
}

void Camera::AddTrauma(float t)
{
    if (m_Amplitude <= 0.f || m_Frequency <= 0.f)
    {
        m_Amplitude = 18.f;    
        m_Frequency = 7.f;      
        m_ShakeDuration = 0.25f;   
        m_Falloff = 1.2f;
        m_ShakeTime = 0.f;
    }

    m_Trauma = clamp(m_Trauma + t, 0.f, 1.f);

}

void Camera::StopShake()
{
    m_Amplitude = 0.f; m_Frequency = 0.f;
    m_ShakeTime = 0.f; m_ShakeDuration = 0.f;
    m_Trauma = 0.f;
    m_ShakeOffset = Vector2(0.f,0.f);
    m_ShakeRot = 0.f;
}

void Camera::UpdateShake()
{
    float dt = DELTA;
    m_ShakeTime += dt;

    // 트라우마 자연 감소
    if (m_Trauma > 0.f)
        m_Trauma = max(0.f, m_Trauma - m_TraumaDecay * dt);

    // 지속시간 감쇠
    float durFactor = 1.f;
    if (m_ShakeDuration > 0.f)
    {
        float t = clamp(m_ShakeTime / m_ShakeDuration, 0.f, 1.f);
        durFactor = 1.f - powf(t, m_Falloff);
        if (t >= 1.f && m_Trauma <= 0.f) StopShake();
    }

    float traumaPow = m_Trauma * m_Trauma;
    float finalAmp = m_Amplitude * durFactor * (0.3f + 0.7f * traumaPow);

    if (finalAmp <= 0.001f || m_Frequency <= 0.001f)
    {
        m_ShakeOffset = Vector2(0,0);
        m_ShakeRot = 0.f;
        return;
    }

    float tt = m_ShakeTime * m_Frequency;
    Vector2 n = SmoothNoise2D(tt, m_SeedX, m_SeedY); // [-1,1]
    m_ShakeOffset = n * finalAmp;

    float r = SmoothNoise1D(tt, m_SeedR);
    m_ShakeRot = r * (finalAmp * 0.0025f); // 매우 작게(옵션)
}

float Camera::SmoothNoise1D(float t, float seed) const
{
    float a = sinf((t + seed * 0.37f) * 2.f * PI * 0.5f);
    float b = sinf((t + seed * 0.73f) * 2.f * PI * 0.25f);
    return (a * 0.6f + b * 0.4f); // [-1,1] 근사
}

Vector2 Camera::SmoothNoise2D(float t, float seedX, float seedY) const
{
    return { SmoothNoise1D(t, seedX), SmoothNoise1D(t + 100.f, seedY) };
}
