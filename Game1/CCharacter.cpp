#include "stdafx.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"

CCharacter::CCharacter()
{
	m_Status = CHelper::CreateComponent<CStatusComponent>(this);
}

CCharacter::~CCharacter()
{
}

