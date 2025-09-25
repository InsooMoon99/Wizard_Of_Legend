#include "stdafx.h"
#include "CUI_PlayerHUD.h"

#include "CUI_Inventory.h"
#include "CUI_PlayerStatus.h"
#include "CUI_Skill_Pannel.h"

CUI_PlayerHUD::CUI_PlayerHUD()
{
	m_PlayerStatus = make_shared<CUI_PlayerStatus>();
	m_Skill_Pannel = make_shared<CUI_Skill_Pannel>();
	m_Inventory	   = make_shared<CUI_Inventory>();

	AddChild(m_PlayerStatus);
	AddChild(m_Skill_Pannel);
	AddChild(m_Inventory);

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
