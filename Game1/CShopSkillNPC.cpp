#include "stdafx.h"
#include "CShopSkillNPC.h"

#include "CItem.h"
#include "CShopComponent.h"
#include "CStateComponent.h"

CShopSkillNPC::CShopSkillNPC()
{
	m_Shop = CHelper::CreateSharedComponent<CShopComponent>(this);


	Resource_Settings();
}

CShopSkillNPC::~CShopSkillNPC()
{
	Release();

}

void CShopSkillNPC::BeginPlay()
{
	CCharacter::BeginPlay();

	m_Shop->Equip_ItemById("Meteor");

}

void CShopSkillNPC::Tick()
{
	CCharacter::Tick();

	for (auto& col : m_vecCol)
		col->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& component : m_Components)
		component->TickComponent();
}

void CShopSkillNPC::LateUpdate()
{
	CCharacter::LateUpdate();

}

void CShopSkillNPC::Render()
{
	CCharacter::Render();

	for (auto& col : m_vecCol)
		col->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& component : m_Components)
		component->Render();
}

void CShopSkillNPC::Release()
{
	CCharacter::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
}

void CShopSkillNPC::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CCharacter::OnComponentBeginOverlap(collider, other);

}

void CShopSkillNPC::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CCharacter::OnComponentEndOverlap(collider, other);

}

void CShopSkillNPC::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	CCharacter::SetSprite(_State, _Dir);

	if (m_CurImg)
		m_CurImg->visible = false;

	auto& stateIter = m_Sprites[_State];
	auto imgIter = stateIter.find(_Dir);

	if (imgIter != stateIter.end())
	{
		m_CurImg = imgIter->second;
		m_CurImg->visible = true;

		Play_Sprite(_State);
	}

}

void CShopSkillNPC::Play_Sprite(ECharacterState _State)
{
	CCharacter::Play_Sprite(_State);

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		break;
	}

}


void CShopSkillNPC::Resource_Settings()
{
	CCharacter::Resource_Settings();

	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f);
	m_Collision->SetWorldPos(Vec2(2116.f, -2270.f));
	m_Collision->visible = false;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	//COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	// NPC Idle
	{
		Vec2 idle_Size = Vec2(24.f, 40.f) * 3.5f;
		m_IdleImg = RESOURCE->CreateImage(L"Shop\\Vendor.png", idle_Size, true);
		m_IdleImg->SetParentRT(*m_Collision);
		//front_Idle->SetLocalPos(Vec2(-7.f, 10.f));

		m_vecCol.push_back(m_IdleImg.get());

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = m_IdleImg;

	}
	// µ¾ÀÚ¸®
	{
		m_Mat = RESOURCE->CreateImage(L"Shop\\µ¾ÀÚ¸®.bmp", Vec2(577.f, 198.f), true);
		m_Mat->SetParentRT(*m_Collision);
		m_Mat->SetLocalPos(Vec2(30.f, -140.f));

		m_vecCol.push_back(m_Mat.get());
	}
	
	m_CurImg = m_Sprites[ECharacterState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}
