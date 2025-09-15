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

void CCharacter::AddComponent(CComponent* _Component)
{
	m_Components.push_back(_Component);
}

void CCharacter::RemoveComponent(CComponent* _Component)
{
	auto iter = remove(m_Components.begin(), m_Components.end(), _Component);

	if (iter != m_Components.end())
	{
		m_Components.erase(iter, m_Components.end());
	}
}
