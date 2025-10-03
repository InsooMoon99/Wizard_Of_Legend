#include "stdafx.h"
#include "CItem_Gold.h"

#include "CFloatingText.h"
#include "CInventoryComponent.h"
#include "CPlayer.h"
#include "CStatusComponent.h"

CItem_Gold::CItem_Gold()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f);
	//m_Collision->SetWorldPos(Vec2(2270.f, -2426.f));
	m_Collision->visible = false;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);

	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	m_RenderLayer = ERenderLayer::Render_Effect;

	m_ItemImg = RESOURCE->CreateSprite(L"Shop\\Gold.png", Vec2(80.f / 4.f, 20.f), Int2(4,1), true);
	m_ItemImg->SetParentRT(*m_Collision);
	m_ItemImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
}

CItem_Gold::~CItem_Gold()
{
	Release();
}

void CItem_Gold::BeginPlay()
{
	CItem::BeginPlay();

}

void CItem_Gold::Tick()
{
	CItem::Tick();

	for (auto& iter : m_vecCol)
	{
		if (iter) 
			iter->Update();
	}

	if (m_Attracting && m_Target && m_Collision)
	{
		Vec2 myPos = m_Collision->GetWorldPos();
		Vec2 targetPos = m_Target->Get_Collision()->GetWorldPos();

		Vec2 dir = targetPos - myPos;
		float dist = dir.Length();

		if (dist < 1e-3f) return;
		dir.Normalize();

		float move = m_Speed * DELTA * 1.5f;
		if (dist - 50.f <= move)
		{
			ActivateItem(m_Target);
		}
		else
		{
			m_Collision->MoveWorldPos(dir * move);
		}
	}
}

void CItem_Gold::Render()
{
	CItem::Render();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Render();
	}
}

void CItem_Gold::LateUpdate()
{
	CItem::LateUpdate();

}

void CItem_Gold::Release()
{
	CItem::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}

	m_vecCol.clear();
}

void CItem_Gold::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentBeginOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	m_Attracting = true;
	m_Target = player;


}

void CItem_Gold::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentEndOverlap(collider, other);

}

void CItem_Gold::ActivateItem(CActor* actor)
{
	CItem::ActivateItem(actor);

	CPlayer* player = dynamic_cast<CPlayer*>(actor);
	if (!player) return;

	auto status = CHelper::GetComponent<CStatusComponent>(player);
	if (!status) return;

	// 골드 획득
	int current = status->GetGold();
	status->SetGold(current + 10);
	SOUND->StopPlay("GoldSpawn", 0.2f);

	FFloatingTextDesc text;
	text.text = L"+" + to_wstring(10);
	text.startPos = m_Collision->GetWorldPos();
	text.duration = 0.9f;
	text.rise = 70.f;
	text.fontSize = 30.f;
	text.fontStyle = L"PixelMplus10";

	auto font = make_shared<CFloatingText>(text);
	OBJECT->SpawnActor(font);

	DisableAndDestroy();
}
