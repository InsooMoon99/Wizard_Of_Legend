#include "stdafx.h"
#include "CUI_PlayerHUD.h"
#include "CUI_PlayerStatus.h"

CUI_PlayerHUD::CUI_PlayerHUD()
{
	m_PlayerStatus = make_shared<CUI_PlayerStatus>();

	AddChild(m_PlayerStatus);


}

CUI_PlayerHUD::~CUI_PlayerHUD()
{
	Release();
}

void CUI_PlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	
}

void CUI_PlayerHUD::Tick()
{
	Super::Tick();

	
}

void CUI_PlayerHUD::Render()
{
	Super::Render();


}

void CUI_PlayerHUD::Release()
{
	Super::Release();

	
}
