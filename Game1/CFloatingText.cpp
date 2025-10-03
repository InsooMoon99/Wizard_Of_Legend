#include "stdafx.h"
#include "CFloatingText.h"
#include <algorithm>

CFloatingText::CFloatingText(const FFloatingTextDesc& desc)
    : m_Desc(desc)
{
    m_RenderLayer = ERenderLayer::Render_Effect;
    m_WorldPos = desc.startPos; 
}

void CFloatingText::BeginPlay()
{
    CActor::BeginPlay();
    m_Scale = 1.25f;
}

void CFloatingText::Tick()
{
    CActor::Tick();

    m_Elapsed += DELTA;
    float t = clamp(m_Elapsed / m_Desc.duration, 0.f, 1.f);
    float rise = -m_Desc.rise * EaseOutCubic(t);
    m_WorldPos = m_Desc.startPos;
    m_WorldPos.y -= rise;

    if (t >= 0.6f) {
        float k = (t - 0.6f) / 0.4f;
        m_Alpha = clamp(1.f - k, 0.f, 1.f);
    }
    else {
        m_Alpha = 1.f;
    }

    if (m_Elapsed <= 0.2f) {
        float k = clamp(m_Elapsed / 0.2f, 0.f, 1.f);
        m_Scale = 1.25f - 0.25f * k;
    }
    else {
        m_Scale = 1.0f;
    }

    if (t >= 1.f)
        m_PendingKill = true;
}

void CFloatingText::Render()
{
    CActor::Render();

    Vector2 cam = CAM->GetCameraPos();
    Vector2 half = Vector2(app.GetWidth() * 0.5f, app.GetHeight() * 0.5f);

    Vector2 screenPos;
    screenPos.x = (m_WorldPos.x - cam.x) + half.x;
    screenPos.y = -(m_WorldPos.y - cam.y) + half.y;

    float size = m_Desc.fontSize * m_Scale;

    RECT rect
	{
        (LONG)(screenPos.x + 40.f),
        (LONG)(screenPos.y - size * 1.1f),
        (LONG)(screenPos.x + 120.f),
        (LONG)(screenPos.y + size * 0.3f)
    };

    if (m_Desc.outline)
    {
        const float t = m_Desc.outlineThickness;
        const POINT offs[8] = 
        {
            {-(LONG)t, 0}, {(LONG)t, 0}, {0, -(LONG)t}, {0, (LONG)t},
            {-(LONG)t, -(LONG)t}, {-(LONG)t, (LONG)t}, {(LONG)t, -(LONG)t}, {(LONG)t, (LONG)t}
        };

        Color oc = m_Desc.outlineColor;

        for (auto& o : offs)
        {
            RECT r = rect;
            r.left += o.x; r.right += o.x;
            r.top += o.y; r.bottom += o.y;

            DWRITE->RenderText(
                m_Desc.text.c_str(),
                r,
                size,
                m_Desc.fontStyle.c_str(),
                oc,
                DWRITE_FONT_WEIGHT_BOLD
            );
        }
    }


    DWRITE->RenderText(
        m_Desc.text.c_str(),
        rect,
        size,
        m_Desc.fontStyle, 
        Color(m_Desc.color.R(), m_Desc.color.G(), m_Desc.color.B(), m_Alpha),
        DWRITE_FONT_WEIGHT_BOLD
    );
}

void CFloatingText::LateUpdate()
{
    CActor::LateUpdate();
}

void CFloatingText::Release()
{
    CActor::Release();
}
