#pragma once
#include "CUI.h"

class CPlayer;

class CUI_Minimap : public CUI
{
public:
    explicit CUI_Minimap(const shared_ptr<CPlayer>& player);
    ~CUI_Minimap() override;

public:
    void BeginPlay() override;
    void Tick() override;
    void LateUpdate() override;
    void Render() override;
    void Release() override;

    void SetScreenPos(const Vec2& pos) { m_ScreenPos = pos; if (m_Background) m_Background->SetWorldPos(m_ScreenPos); }
    void SetMiniSize(const Vec2& size) { m_MiniSize = size; if (m_Background) m_Background->scale = m_MiniSize; }
    void SetMapSize(const Vec2& mapSize) { m_MapSize = mapSize; m_WorldMax = m_WorldMin + m_MapSize; }
    void SetWorldBounds(const Vec2& worldMin, const Vec2& worldMax) { m_WorldMin = worldMin; m_WorldMax = worldMax; m_MapSize = m_WorldMax - m_WorldMin; }

private:
    Vec2 ToMini_Local_Centered(const Vec2& world) const;

private:
    shared_ptr<ObImage> m_Background;
    shared_ptr<ObImage> m_PlayerIcon;

    weak_ptr<CPlayer>   m_Player;

    Vec2 m_ScreenPos = Vec2(820.f, 420.f);  
    Vec2 m_MiniSize = Vec2(200.f, 200.f);   

    Vec2 m_WorldMin = Vec2(0.f, 0.f);                  
    Vec2 m_WorldMax = Vec2(2304.f * 3.f, 2560.f * 3.f);
    Vec2 m_MapSize = Vec2(2304.f * 3.f, 2560.f * 3.f); 
};
