#include "stdafx.h"
#include "CAIComponent.h"

CAIComponent::CAIComponent()
{

}

CAIComponent::~CAIComponent()
{
	Release();
}

void CAIComponent::BeginPlay()
{
	CComponent::BeginPlay();

	if (m_Owner)
		m_Context.Owner = m_Owner;
}

void CAIComponent::TickComponent()
{
	CComponent::TickComponent();

	if (!m_RootNode) return;

	m_Target = dynamic_cast<CMonsterBase*>(m_Owner)->GetTarget().get();
	m_Context.Blackboard.SetValueAsActor(BBKey::Target, m_Target); // Blackboard에 값 동기화

	m_RootNode->Tick(m_Context);
}

void CAIComponent::Render()
{
	CComponent::Render();

}

void CAIComponent::Release()
{
	CComponent::Release();

	m_RootNode.reset();
}
