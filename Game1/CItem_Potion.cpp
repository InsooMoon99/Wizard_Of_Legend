#include "stdafx.h"
#include "CItem_Potion.h"

#include "CPlayer.h"
#include "CStatusComponent.h"

CItem_Potion::CItem_Potion()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f);
	m_Collision->SetWorldPos(Vec2(1970.f, -2426.f));
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);

	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	m_RenderLayer = ERenderLayer::Render_Effect;
}

CItem_Potion::~CItem_Potion()
{
	Release();
}

void CItem_Potion::BeginPlay()
{
	CItem::BeginPlay();

	m_ItemImg->scale = m_Size * 2.f;
	m_ItemImg->SetParentRT(*m_Collision);
}

void CItem_Potion::Tick()
{
	CItem::Tick();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Update();
	}
}

void CItem_Potion::Render()
{
	CItem::Render();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Render();
	}
}

void CItem_Potion::LateUpdate()
{
	CItem::LateUpdate();

}

void CItem_Potion::Release()
{
	CItem::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}

	m_vecCol.clear();
}

void CItem_Potion::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentBeginOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	auto self = shared_from_this();
	player->AddInteract((self));

}

void CItem_Potion::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentEndOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	player->RemoveInteract(this);
}

void CItem_Potion::ActivateItem(CActor* actor)
{
	CItem::ActivateItem(actor);

	CPlayer* player = dynamic_cast<CPlayer*>(actor);
	if (!player) return;

	auto status = CHelper::GetComponent<CStatusComponent>(player);
	if (!status) return;

	bool bGoldCheck = status->GetGold() >= m_ItemData.Price;
	if (!bGoldCheck) return;

	int hp = status->GetHp();
	status->SetHp(hp + m_ItemData.Value);
	player->RemoveInteract(this);

	DisableAndDestroy();
}
