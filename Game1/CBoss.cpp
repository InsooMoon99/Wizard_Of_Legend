#include "stdafx.h"
#include "CBoss.h"

#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "CStatusComponent.h"
#include "CUIComponent.h"

CBoss::CBoss()
{
	m_Weapon = CHelper::CreateComponent<CWeaponComponent>(this);
	//m_UI = CHelper::CreateComponent<CUIComponent>(this);

	m_Status->SetMaxHp(100.f);
	m_Status->SetMaxMp(100.f);

	m_Status->SetHp(80.f);
	m_Status->SetMp(60.f);
	m_Status->SetSpeed(500.f);

	Resource_Settings();
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::BeginPlay()
{
	CCharacter::BeginPlay();

	//if (m_UI)
	//	m_UI->SetBossStatus();


}

void CBoss::Tick()
{
	CCharacter::Tick();

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	m_prevState = m_State->Get_State();
	m_prevDir = m_State->Get_Dir();

	switch (m_State->Get_State())
	{
	case EPlayerState::Idle:
		State_Idle();
		break;

	case EPlayerState::Walk:
		State_Walk();
		break;

	case EPlayerState::Attack1:
		State_Attack();
		break;
	}

	if (m_prevState != m_State->Get_State() || m_prevDir != m_State->Get_Dir())
	{
		SetSprite(m_State->Get_State(), m_State->Get_Dir());
	}

	// Color 변경
	Update_Hit();

	// Update
	for (auto& col : m_vecCol)
		col->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& component : m_Components)
		component->TickComponent();
}

void CBoss::LateUpdate()
{
	CCharacter::LateUpdate();

}

void CBoss::Render()
{
	CCharacter::Render();

	for (auto& col : m_vecCol)
		col->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& component : m_Components)
		component->Render();
}

void CBoss::Release()
{
	CCharacter::Release();

	m_Sprites.clear();

	m_CurImg.reset();

	for (auto& component : m_Components)
		SafeDelete(component);

	for (auto col : m_vecCol)
		SafeDelete(col);
}

void CBoss::Update_Hit()
{
	CCharacter::Update_Hit();

	if (m_HitTimer > 0.f)
	{
		m_HitTimer -= DELTA;

		if (m_HitTimer <= 0.f)
		{
			CHelper::OrignColor(Get_CurImage().get());
		}
	}
}

void CBoss::Hit_Color()
{
	CCharacter::Hit_Color();

	CHelper::HitColor(Get_CurImage().get());
	m_HitTimer = 0.15f;
}

void CBoss::Hit_Effect()
{
	CCharacter::Hit_Effect();

}

void CBoss::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CCharacter::OnComponentBeginOverlap(collider, other);

	// Boss Hit
	cout << "Boss Hit" << endl;
}

void CBoss::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CCharacter::OnComponentEndOverlap(collider, other);

}

void CBoss::State_Idle()
{
	// TODO : 공격 대기 시간
}

void CBoss::State_Walk()
{
	// TODO : 플레이어 추격
}

void CBoss::State_Attack()
{
	// TODO : 플레이어 공격
}

void CBoss::SetSprite(EPlayerState _State, EPlayerDir _Dir)
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

void CBoss::Play_Sprite(EPlayerState _State)
{
	CCharacter::Play_Sprite(_State);

	switch (_State)
	{
	case EPlayerState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case EPlayerState::Walk:
		//m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case EPlayerState::Attack1:
		//m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.06f);
		//m_CurImg->frame = Int2(0, 0);
		break;

	}
}

void CBoss::Resource_Settings()
{
	CCharacter::Resource_Settings();

	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(80.f, 80.f);
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->pivot = OFFSET_N;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	// State Idle
	{
		Vec2 idle_Size = Vec2(150.f / 3.f, 50.f) * 5.f;
		shared_ptr<ObImage> bat_Idle = RESOURCE->CreateSprite(L"Temp_Lightning\\Boss_Bat_Idle.png", idle_Size, Int2(3, 1), true);
		bat_Idle->SetParentRT(*m_Collision);

		m_Sprites[EPlayerState::Idle][EPlayerDir::Front] = bat_Idle;
	}

	m_CurImg = m_Sprites[EPlayerState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}