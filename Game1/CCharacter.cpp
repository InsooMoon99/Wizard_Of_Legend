#include "stdafx.h"
#include "CCharacter.h"

CCharacter::CCharacter() : m_Hp(0.f), m_Speed(0.f)
{
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
