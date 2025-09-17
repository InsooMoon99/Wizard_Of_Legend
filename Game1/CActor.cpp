#include "stdafx.h"
#include "CActor.h"

#include "CComponent.h"

CActor::CActor()
{

}

CActor::~CActor()
{

}

void CActor::BeginPlay()
{
	for (auto* component : m_Components)
	{
		component->BeginPlay();
	}
}

void CActor::Tick()
{
	for (auto* component : m_Components)
	{
		component->TickComponent();
	}
}

void CActor::Render()
{
	for (auto* component : m_Components)
	{
		component->Render();
	}
}

void CActor::Release()
{
	// TODO : Component Release?

}

void CActor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{

}

void CActor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{

}

void CActor::AddComponent(CComponent* _Component)
{
	if (_Component == nullptr)
		return;

	_Component->SetOwner(this);
	m_Components.push_back(_Component);
}

void CActor::RemoveComponent(CComponent* _Component)
{
	auto find_iter = find(m_Components.begin(), m_Components.end(), _Component);
	if (find_iter == m_Components.end())
		return;

	m_Components.erase(find_iter);
}
