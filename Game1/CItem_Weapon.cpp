#include "stdafx.h"
#include "CItem_Weapon.h"

#include "CPlayer.h"
#include "CStatusComponent.h"
#include "CInventoryComponent.h"

CItem_Weapon::CItem_Weapon()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f);
	m_Collision->SetWorldPos(Vec2(2120.f, -2426.f));
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);

	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	m_RenderLayer = ERenderLayer::Render_Effect;
}

CItem_Weapon::~CItem_Weapon()
{
	Release();
}

void CItem_Weapon::BeginPlay()
{
	CItem::BeginPlay();

	m_ItemImg->scale = m_Size * 2.f;
	m_ItemImg->SetParentRT(*m_Collision);
}

void CItem_Weapon::Tick()
{
	CItem::Tick();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Update();
	}
}

void CItem_Weapon::Render()
{
	CItem::Render();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Render();
	}
}

void CItem_Weapon::LateUpdate()
{
	CItem::LateUpdate();

}

void CItem_Weapon::Release()
{
	CItem::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}

	m_vecCol.clear();
}

void CItem_Weapon::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentBeginOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	auto self = shared_from_this();
	player->AddInteract((self));

}

void CItem_Weapon::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentEndOverlap(collider, other);

}

void CItem_Weapon::ActivateItem(CActor* actor)
{
	CItem::ActivateItem(actor);

	CPlayer* player = dynamic_cast<CPlayer*>(actor);
	if (!player) return;

	auto status = CHelper::GetComponent<CStatusComponent>(player);
	if (!status) return;

	int attack = status->GetAttack();
	status->SetAttack(attack * m_ItemData.Value);

	auto inven = CHelper::GetComponent<CInventoryComponent>(player);
	if (!inven) return;

	auto self = shared_from_this();
	inven->AddItem(self);

	player->RemoveInteract(this);

	DisableAndDestroy();
}
