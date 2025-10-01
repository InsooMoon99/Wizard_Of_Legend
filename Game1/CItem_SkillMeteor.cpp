#include "stdafx.h"
#include "CItem_SkillMeteor.h"

#include "CPlayer.h"
#include "CStatusComponent.h"
#include "CWeaponComponent.h"

CItem_SkillMeteor::CItem_SkillMeteor()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f);
	m_Collision->SetWorldPos(Vec2(-100.f));
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);

	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	m_RenderLayer = ERenderLayer::Render_Effect;

	//m_ItemImg = RESOURCE->CreateImage(L"Shop\\MeteorCard.png", Vec2(32 ,50) * 2.f, true);
	//m_ItemImg->SetParentRT(*m_Collision);

}

CItem_SkillMeteor::~CItem_SkillMeteor()
{
	Release();
}

void CItem_SkillMeteor::BeginPlay()
{
	CItem::BeginPlay();

	m_ItemImg->scale = m_Size * 2.f;
	m_ItemImg->SetParentRT(*m_Collision);

	m_PriceTag = RESOURCE->CreateImage(L"Shop\\PriceMarkerGold.png", Vec2(22.f, 10.f) * 3.f, true);
	m_PriceTag->SetParentRT(*m_ItemImg);
	m_PriceTag->SetLocalPos(Vec2(0.f, -70.f));

	m_Price = to_wstring(m_ItemData.Price);
}

void CItem_SkillMeteor::Tick()
{
	CItem::Tick();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Update();
	}

	m_PriceTag->Update();
}

void CItem_SkillMeteor::Render()
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

void CItem_SkillMeteor::LateUpdate()
{
	CItem::LateUpdate();

}

void CItem_SkillMeteor::Release()
{
	CItem::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}

	m_vecCol.clear();
}

void CItem_SkillMeteor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentBeginOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	auto self = shared_from_this();
	player->AddInteract((self));

}

void CItem_SkillMeteor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CItem::OnComponentEndOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	player->RemoveInteract(this);
}

void CItem_SkillMeteor::ActivateItem(CActor* actor)
{
	CItem::ActivateItem(actor);

	CPlayer* player = dynamic_cast<CPlayer*>(actor);
	if (!player) return;

	auto weapon = CHelper::GetComponent<CWeaponComponent>(player);
	if (!weapon) return;

	auto status = CHelper::GetComponent<CStatusComponent>(player);
	if (!status) return;

	bool bGoldCheck = status->GetGold() >= m_ItemData.Price;
	if (!bGoldCheck) return;
	
	status->SetGold(status->GetGold() - m_ItemData.Price);

	weapon->Equip_Skill(ESkillType::Fire_Meteor, ESkillSlot::R);
	PARTICLE->Spawn_Particle(L"BuyCard", m_Collision->GetWorldPos());
	SOUND->StopPlay("DropSpell", 0.5f);

	player->RemoveInteract(this);

	DisableAndDestroy();
}
