#include "stdafx.h"
#include "CItem_Armor.h"

#include "CInventoryComponent.h"
#include "CPlayer.h"
#include "CStatusComponent.h"

CItem_Armor::CItem_Armor()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f);
	m_Collision->SetWorldPos(Vec2(2270.f, -2426.f));
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);

	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	m_RenderLayer = ERenderLayer::Render_Effect;
}

CItem_Armor::~CItem_Armor()
{
	Release();
}

void CItem_Armor::BeginPlay()
{
	CItem::BeginPlay();

	m_ItemImg->scale = m_Size * 2.f;
	m_ItemImg->SetParentRT(*m_Collision);
}

void CItem_Armor::Tick()
{
	CItem::Tick();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Update();
	}
}

void CItem_Armor::Render()
{
	CItem::Render();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Render();
	}
}

void CItem_Armor::LateUpdate()
{
	CItem::LateUpdate();

}

void CItem_Armor::Release()
{
	CItem::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}

	m_vecCol.clear();
}

void CItem_Armor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentBeginOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	auto self = shared_from_this();
	player->AddInteract((self));

}

void CItem_Armor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentEndOverlap(collider, other);

}

void CItem_Armor::ActivateItem(CActor* actor)
{
	CItem::ActivateItem(actor);

	CPlayer* player = dynamic_cast<CPlayer*>(actor);
	if (!player) return;

	auto status = CHelper::GetComponent<CStatusComponent>(player);
	if (!status) return;

	int def = status->GetDef();
	status->SetDef(def + m_ItemData.Value);

	auto inven = CHelper::GetComponent<CInventoryComponent>(player);
	if (!inven) return;

	auto self = shared_from_this();
	inven->AddItem(self);

	player->RemoveInteract(this);

	DisableAndDestroy();
}
