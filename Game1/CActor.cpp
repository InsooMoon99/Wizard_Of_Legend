#include "stdafx.h"
#include "CActor.h"

#include "CComponent.h"

CActor::CActor() : CGameObject(ERenderLayer::Render_GameObject)
{

}

CActor::~CActor()
{

}

void CActor::BeginPlay()
{
	for (auto component : m_Components)
	{
		component->BeginPlay();
	}

	ApplyPendingSpawnPos();
}

void CActor::Tick()
{
	for (auto component : m_Components)
	{
		component->TickComponent();
	}
}

void CActor::Render()
{
	for (auto component : m_Components)
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

void CActor::ApplyPendingSpawnPos()
{
	if (!m_HasPendingSpawnPos) return;

	Set_WorldPos(m_PendingSpawnPos);
	m_HasPendingSpawnPos = false;
}

void CActor::AddComponent(shared_ptr<CComponent> _Component)
{
	if (_Component == nullptr)
		return;

	_Component->SetOwner(this);
	m_Components.push_back(_Component);
}

void CActor::RemoveComponent(CComponent* target)
{
	auto it = find_if(m_Components.begin(), m_Components.end(),
		[target](const std::shared_ptr<CComponent>& sp) {
			return sp.get() == target;
		});
	if (it != m_Components.end())
		m_Components.erase(it);
}

