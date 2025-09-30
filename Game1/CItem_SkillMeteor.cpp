#include "stdafx.h"
#include "CItem_SkillMeteor.h"

#include "CPlayer.h"
#include "CStatusComponent.h"
#include "CWeaponComponent.h"

CItem_SkillMeteor::CItem_SkillMeteor()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f);
	//m_Collision->SetWorldPos(Vec2(1970.f, -2426.f));
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);

	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	m_RenderLayer = ERenderLayer::Render_Effect;

	m_ItemImg = RESOURCE->CreateImage(L"Shop\\MeteorCard.png", Vec2(32 ,50) * 2.f, true);
	m_ItemImg->SetParentRT(*m_Collision);


}

CItem_SkillMeteor::~CItem_SkillMeteor()
{
	Release();
}

void CItem_SkillMeteor::BeginPlay()
{
	CItem::BeginPlay();

}

void CItem_SkillMeteor::Tick()
{
	CItem::Tick();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Update();
	}
}

void CItem_SkillMeteor::Render()
{
	CItem::Render();

	for (auto& iter : m_vecCol)
	{
		if (iter)
			iter->Render();
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

}

void CItem_SkillMeteor::ActivateItem(CActor* actor)
{
	CItem::ActivateItem(actor);

	CPlayer* player = dynamic_cast<CPlayer*>(actor);
	if (!player) return;

	auto status = CHelper::GetComponent<CStatusComponent>(player);
	if (!status) return;

	auto weapon = CHelper::GetComponent<CWeaponComponent>(player);
	if (!weapon) return;

	int hp = status->GetHp();
	status->SetHp(hp + m_ItemData.Value);

	weapon->Equip_Skill(ESkillType::Fire_Meteor, ESkillSlot::R);

	player->RemoveInteract(this);

	DisableAndDestroy();
}
