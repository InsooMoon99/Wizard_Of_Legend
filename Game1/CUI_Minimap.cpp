#include "stdafx.h"
#include "CUI_Minimap.h"
#include "CResourceManager.h"
#include "CPlayer.h"

CUI_Minimap::CUI_Minimap(const shared_ptr<CPlayer>& player)
{
    m_Player = player;

    m_RenderLayer = ERenderLayer::Render_UI;
}

CUI_Minimap::~CUI_Minimap()
{
    Release();
}

void CUI_Minimap::BeginPlay()
{
    CUI::BeginPlay();

    m_Background = RESOURCE->CreateUI(L"UI\\minimap_200_box.png", m_MiniSize, true);
    m_Background->pivot = OFFSET_N;         
    m_Background->space = SPACE::SCREEN;
    m_Background->color.A(0.2f);

    const Vec2 half(app.GetHalfWidth(), app.GetHalfHeight());
    const float padding = 16.f; 

    const Vec2 anchorBR( 
        +half.x - m_MiniSize.x * 0.5f - padding,
        +half.y - m_MiniSize.y * 0.5f - padding 
    );

    m_Background->SetWorldPos(anchorBR);

    m_PlayerIcon = RESOURCE->CreateUI(L"UI\\PLAYER_MINIMAP.bmp", Vec2(10.f, 10.f), true);
    m_PlayerIcon->pivot = OFFSET_N;          
    m_PlayerIcon->space = SPACE::SCREEN;
    m_PlayerIcon->SetParentRT(*m_Background);

    if (m_MapSize.x <= 0.f || m_MapSize.y <= 0.f)
        m_MapSize = Vec2(2304.f * 3.f, 2560.f * 3.f);
    if (m_WorldMax == Vec2())
        m_WorldMax = m_WorldMin + m_MapSize;
}

void CUI_Minimap::Tick()
{
    CUI::Tick();

    if (auto sp = m_Player.lock())
    {
        const Vec2 local = ToMini_Local_Centered(sp->Get_Collision()->GetWorldPos());
        m_PlayerIcon->SetLocalPos(local);
    }

    if (m_Background)  m_Background->Update();
    if (m_PlayerIcon)  m_PlayerIcon->Update();
}

void CUI_Minimap::LateUpdate()
{
    CUI::LateUpdate();
}

void CUI_Minimap::Render()
{
    CUI::Render();

    if (m_Background)  m_Background->Render();
    if (m_PlayerIcon)  m_PlayerIcon->Render();
}

void CUI_Minimap::Release()
{
    m_PlayerIcon.reset();
    m_Background.reset();
}

Vec2 CUI_Minimap::ToMini_Local_Centered(const Vec2& w) const
{
    const float dx = max(1e-6f, m_WorldMax.x - m_WorldMin.x);
    const float dy = max(1e-6f, m_WorldMax.y - m_WorldMin.y);

    Vec2 n;
    n.x = (w.x - m_WorldMin.x) / dx;   
    n.y = (w.y - m_WorldMin.y) / dy;

    n.x = clamp(n.x, 0.f, 1.f);
    n.y = clamp(n.y, 0.f, 1.f);

    Vec2 local = (n * m_MiniSize) - (m_MiniSize * 0.5f);

    const float pad = 6.f; 
    local.x = clamp(local.x, -m_MiniSize.x * 0.5f + pad, m_MiniSize.x * 0.5f - pad);
    local.y = clamp(local.y, -m_MiniSize.y * 0.5f + pad, m_MiniSize.y * 0.5f - pad);

    return local;
}
