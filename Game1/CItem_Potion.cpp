#include "stdafx.h"
#include "CItem_Potion.h"

#include "CPlayer.h"
#include "CStatusComponent.h"

CItem_Potion::CItem_Potion()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f, 200.f);
	m_Collision->pivot = OFFSET_T;
	m_Collision->SetWorldPos(Vec2(1990.f, -2324.f));
	m_Collision->visible = false;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);

	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

}

CItem_Potion::~CItem_Potion()
{
	Release();
}

void CItem_Potion::BeginPlay()
{
	CItem::BeginPlay();

	m_ItemImg->scale = m_Size * 1.5f;
	m_ItemImg->SetParentRT(*m_Collision);

	m_PriceTag = RESOURCE->CreateImage(L"Shop\\PriceMarkerGold.png", Vec2(22.f, 10.f) * 3.f, true);
	m_PriceTag->SetParentRT(*m_ItemImg);
	m_PriceTag->SetLocalPos(Vec2(0.f, -45.f));

	m_Price = to_wstring(m_ItemData.Price);
}

void CItem_Potion::Tick()
{
	CItem::Tick();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Update();
	}

	m_PriceTag->Update();
}

void CItem_Potion::Render()
{
	CItem::Render();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Render();
	}

	m_PriceTag->Render();

	if (m_PriceTag && m_PriceTag->visible)
	{
		RECT rc = CHelper::MakeScreenRectFromImage(m_PriceTag.get(), Vec2(26.f, 2.f));

		DWRITE->RenderText(
			m_Price,
			rc,
			22.0f,
			L"PixelMplus10",
			Color(0.f, 0.0f, 0.0f, 1.f)
		);
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

	status->SetGold(status->GetGold() - m_ItemData.Price);

	int hp = status->GetHp();
	status->SetHp(hp + m_ItemData.Value);

	SOUND->StopPlay("BuyItem", 0.2f);

	player->RemoveInteract(this);

	DisableAndDestroy();
}
