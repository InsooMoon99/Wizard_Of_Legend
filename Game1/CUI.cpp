#include "stdafx.h"
#include "CUI.h"

CUI::CUI() : CGameObject(ERenderLayer::Render_UI)
{

	bRender = true;
}

CUI::~CUI()
{
	Release();
}

void CUI::BeginPlay()
{

}

void CUI::Tick()
{

}

void CUI::Render()
{
	if (bRender == false)
		return;
}

void CUI::Release()
{

}


