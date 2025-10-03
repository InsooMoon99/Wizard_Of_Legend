#include "stdafx.h"
#include "CUI_BossHUD.h"

#include "CUI_BossStatus.h"

CUI_BossHUD::CUI_BossHUD()
{
	m_BossStatus = make_shared<CUI_BossStatus>();

	AddChild(m_BossStatus);
}

CUI_BossHUD::~CUI_BossHUD()
{

}

void CUI_BossHUD::BeginPlay()
{
	Super::BeginPlay();
}

void CUI_BossHUD::Tick()
{
	Super::Tick();
}

void CUI_BossHUD::Render()
{
	m_BossStatus->bRender = bRender;

	if (!m_BossStatus->bRender)
	{
		return;
	}

	Super::Render();
}

void CUI_BossHUD::Release()
{
	Super::Release();
}
