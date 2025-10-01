#pragma once

#include "CActor.h"

class CFloatingText : public CActor
{
public:
    CFloatingText(const FFloatingTextDesc& desc);
    ~CFloatingText() override = default;

    void BeginPlay() override;
    void Tick() override;
    void LateUpdate() override;
    void Render() override;
    void Release() override;

private:
    float EaseOutCubic(float t) const
    {
        float u = 1.f - t;
        return 1.f - u * u * u;
    }

private:
    FFloatingTextDesc m_Desc;

    Vector2 m_WorldPos;  
    float   m_Elapsed = 0.f;
    float   m_Scale = 1.0f;
    float   m_Alpha = 1.0f;
};
