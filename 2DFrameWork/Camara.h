#pragma once
class Camera : public Singleton<Camera>
{
private:
    Viewport            viewport;       //뷰포트
    Matrix              V, P, VP;       //View x Proj

public:
    Vector2             GetCameraPos() { return position; }

public:
    //카메라위치
    Vector2             position;

public:
    Camera();
    void          Set();
    void          ResizeScreen();
    const Matrix& GetP() { return P; };
    const Matrix& GetVP() { return VP; };

    // === Shake API & Fields ===
public:
    void StartShake(float amplitude, float frequency, float duration, float falloff = 1.1f);
    void AddTrauma(float t);   // 피격/폭발 때 누적 (0~1)
    void StopShake();

    const Vector2& GetShakeOffset() const { return m_ShakeOffset; }
    float GetShakeRot() const { return m_ShakeRot; } // 선택

private:
    void UpdateShake();

private:
    float   m_ShakeTime = 0.f;
    float   m_ShakeDuration = 0.f;
    float   m_Amplitude = 0.f;   // px
    float   m_Frequency = 6.f;   // Hz
    float   m_Falloff = 1.1f;  // 감쇠 곡률(>=1)
    float   m_Trauma = 0.f;   // 0~1
    float   m_TraumaDecay = 1.5f;  // 초당 감소

    Vector2 m_ShakeOffset = { 0,0 };
    float   m_ShakeRot = 0.f;   // rad (옵션)

    // 간단 노이즈용 시드
    float   m_SeedX = 1234.567f;
    float   m_SeedY = 9876.543f;
    float   m_SeedR = 2468.135f;

    // 부드러운 노이즈(간이)
    float  SmoothNoise1D(float t, float seed) const;
    Vector2 SmoothNoise2D(float t, float seedX, float seedY) const;

};