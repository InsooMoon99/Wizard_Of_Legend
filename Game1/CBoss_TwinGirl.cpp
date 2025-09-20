#include "stdafx.h"
#include "CBoss_TwinGirl.h"

#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "CStatusComponent.h"
#include "CUIComponent.h"

CBoss_TwinGirl::CBoss_TwinGirl(CBoss_Controller* _Controller)
	: CBossPawn(_Controller)
{
	m_Weapon = CHelper::CreateSharedComponent<CWeaponComponent>(this);

	m_State->Change_State(ECharacterState::Boss_Start);

	Resource_Settings();
}

CBoss_TwinGirl::~CBoss_TwinGirl()
{
	Release();
}

void CBoss_TwinGirl::BeginPlay()
{
	Super::BeginPlay();

	//if (m_UI)
	//	m_UI->SetBossStatus();

}

void CBoss_TwinGirl::Tick()
{
	Super::Tick();

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	m_prevState = m_State->Get_State();
	m_prevDir = m_State->Get_Dir();

	switch (m_State->Get_State())
	{
	case ECharacterState::Idle:
		State_Idle();
		break;

	case ECharacterState::Walk:
		State_Walk();
		break;

	case ECharacterState::Attack1:
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

void CBoss_TwinGirl::LateUpdate()
{
	Super::LateUpdate();

}

void CBoss_TwinGirl::Render()
{
	Super::Render();

	for (auto& col : m_vecCol)
		col->Render();

	if (m_CurImg)
		m_CurImg->Render();
}

void CBoss_TwinGirl::Release()
{
	Super::Release();


	m_Sprites.clear();

	m_CurImg.reset();

	for (auto& component : m_Components)
		SafeDelete(component);

	for (auto col : m_vecCol)
		SafeDelete(col);
}

void CBoss_TwinGirl::Update_Hit()
{
	Super::Update_Hit();

	if (m_HitTimer > 0.f)
	{
		m_HitTimer -= DELTA;

		if (m_HitTimer <= 0.f)
		{
			CHelper::OrignColor(Get_CurImage().get());
		}
	}
}

void CBoss_TwinGirl::Hit_Color()
{
	Super::Hit_Color();

	CHelper::HitColor(Get_CurImage().get());
	m_HitTimer = 0.15f;
}

void CBoss_TwinGirl::Hit_Effect()
{
	Super::Hit_Effect();

}

void CBoss_TwinGirl::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CBossPawn::OnComponentBeginOverlap(collider, other);

	// Boss Hit
	cout << "Twin Girl Hit" << endl;
}

void CBoss_TwinGirl::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CBossPawn::OnComponentEndOverlap(collider, other);

}

void CBoss_TwinGirl::State_Idle()
{
	// TODO : 공격 대기 시간
}

void CBoss_TwinGirl::State_Walk()
{
	// TODO : 플레이어 추격
}

void CBoss_TwinGirl::State_Attack()
{
	// TODO : 플레이어 공격
}

void CBoss_TwinGirl::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	CBossPawn::SetSprite(_State, _Dir);

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

void CBoss_TwinGirl::Play_Sprite(ECharacterState _State)
{
	CBossPawn::Play_Sprite(_State);

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case ECharacterState::Walk:
		//m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case ECharacterState::Attack1:
		//m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.06f);
		//m_CurImg->frame = Int2(0, 0);
		break;

	case ECharacterState::Boss_Start:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		break;


	}
}

void CBoss_TwinGirl::Resource_Settings()
{
	CBossPawn::Resource_Settings();

	// 보스 충돌체
	{
		m_Collision = make_shared<ObRect>();
		m_Collision->scale = Vec2(80.f, 80.f);
		m_Collision->visible = true;
		m_Collision->isFilled = false;
		m_Collision->pivot = OFFSET_N;
		m_Collision->collider = COLLIDER::RECT;
		m_Collision->Set_Owner(this);
		m_Collision->SetWorldPos(Vec2(-1590.f, -590.f));
		COLLISION->AddCollider(m_Collision.get());

		m_vecCol.push_back(m_Collision.get());
	}

	// Start Setting
	{
		Vec2 start_Size = Vec2(350.f / 7.f, 100.f) * 2.f;
		shared_ptr<ObImage> start = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_GirLand.png", start_Size, Int2(7, 1), true);
		start->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Start][EPlayerDir::Front] = start;
	}

	m_CurImg = m_Sprites[ECharacterState::Boss_Start][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}
