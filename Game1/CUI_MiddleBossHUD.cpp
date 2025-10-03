#include "stdafx.h"
#include "CUI_MiddleBossHUD.h"

#include "CUI_BossStatus.h"
#include "CUI_MiddleBossStatus.h"

CUI_MiddleBossHUD::CUI_MiddleBossHUD()
{
	m_BossStatus = make_shared<CUI_MiddleBossStatus>();

	AddChild(m_BossStatus);
}

CUI_MiddleBossHUD::~CUI_MiddleBossHUD()
{

}

void CUI_MiddleBossHUD::BeginPlay()
{
	Super::BeginPlay();
}

void CUI_MiddleBossHUD::Tick()
{
	Super::Tick();
}

void CUI_MiddleBossHUD::Render()
{
	Super::Render();
}

void CUI_MiddleBossHUD::Release()
{
	Super::Release();
}
