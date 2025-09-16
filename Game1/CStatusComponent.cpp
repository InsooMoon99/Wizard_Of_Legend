#include "stdafx.h"
#include "CStatusComponent.h"

CStatusComponent::CStatusComponent()
{
	m_MaxHp = 100;
	m_Hp = m_MaxHp;

	m_MaxMp = 100.f;
	m_Mp = m_MaxMp;
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

}

void CStatusComponent::Render()
{
	CComponent::Render();

}

void CStatusComponent::Release()
{
	CComponent::Release();

}
