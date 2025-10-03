#include "stdafx.h"
#include "CStatusComponent.h"

CStatusComponent::CStatusComponent()
{
	m_MaxHp = 100;
	m_Hp = m_MaxHp;

	m_MaxMp = 100.f;
	m_Mp = m_MaxMp;

	m_Defense = 10.f;
}

CStatusComponent::~CStatusComponent()
{

}

void CStatusComponent::BeginPlay()
{
	CComponent::BeginPlay();

	
}

void CStatusComponent::TickComponent()
{
	CComponent::TickComponent();

	//m_MaxHp = ::clamp(m_MaxHp, 0.f, 100.f);
	//m_Hp	= ::clamp(m_Hp, 0.f, 100.f);
	//m_MaxMp = ::clamp(m_MaxMp, 0.f, 100.f);
	//m_Mp	= ::clamp(m_Mp, 0.f, 100.f);

	m_Hp = max(0, m_Hp);

}

void CStatusComponent::Render()
{
	CComponent::Render();

}

void CStatusComponent::Release()
{
	CComponent::Release();

}
