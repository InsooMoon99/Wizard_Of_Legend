#include "stdafx.h"
#include "CBoss_TwinMan.h"

#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "CStatusComponent.h"
#include "CUIComponent.h"

CBoss_TwinMan::CBoss_TwinMan(CBoss_Controller* _Controller)
	: CBossPawn(_Controller)
{
	m_Weapon = CHelper::CreateSharedComponent<CWeaponComponent>(this);

	m_State->Change_State(ECharacterState::Boss_Start);

	Resource_Settings();
}

CBoss_TwinMan::~CBoss_TwinMan()
{
	Release();
}

void CBoss_TwinMan::BeginPlay()
{
	Super::BeginPlay();


}

void CBoss_TwinMan::Tick()
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

void CBoss_TwinMan::LateUpdate()
{
	Super::LateUpdate();

}

void CBoss_TwinMan::Render()
{
	Super::Render();

	for (auto& col : m_vecCol)
		col->Render();

	if (m_CurImg)
		m_CurImg->Render();

	// Boss UI Render 부분은 Stage에서 처리하는거로
	//for (auto& component : m_Components)
	//	component->Render();
}

void CBoss_TwinMan::Release()
{
	Super::Release();

	m_Sprites.clear();

	m_CurImg.reset();

	for (auto& component : m_Components)
		SafeDelete(component);

	for (auto col : m_vecCol)
		SafeDelete(col);
}

void CBoss_TwinMan::Update_Hit()
{
	Super::Update_Hit();

	if (m_HitTimer > 0.f)
	{
		m_HitTimer -= DELTA;

		if (m_HitTimer <= 0.f)
		{
			CHelper::OrignColor(m_CurImg.get());
		}
	}
}

void CBoss_TwinMan::Hit_Color()
{
	Super::Hit_Color();

	CHelper::HitColor(m_CurImg.get());
	m_HitTimer = 0.15f;
}

void CBoss_TwinMan::Hit_Effect()
{
	Super::Hit_Effect();

}

void CBoss_TwinMan::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentBeginOverlap(collider, other);

	// Boss Hit
	cout << "Twin Man Hit" << endl;
}

void CBoss_TwinMan::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);

}

void CBoss_TwinMan::State_Idle()
{
	// TODO : 공격 대기 시간
}

void CBoss_TwinMan::State_Walk()
{
	// TODO : 플레이어 추격
}

void CBoss_TwinMan::State_Attack()
{
	// TODO : 플레이어 공격
}

void CBoss_TwinMan::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	Super::SetSprite(_State, _Dir);

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

void CBoss_TwinMan::Play_Sprite(ECharacterState _State)
{
	Super::Play_Sprite(_State);

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

void CBoss_TwinMan::Resource_Settings()
{
	Super::Resource_Settings();

	// 보스 충돌체
	{
		m_Collision = make_shared<ObRect>();
		m_Collision->scale = Vec2(80.f, 80.f);
		m_Collision->visible = true;
		m_Collision->isFilled = false;
		m_Collision->pivot = OFFSET_N;
		m_Collision->collider = COLLIDER::RECT;
		m_Collision->Set_Owner(this);
		m_Collision->SetWorldPos(Vec2(-1790.f, -590.f));
		COLLISION->AddCollider(m_Collision.get());

		m_vecCol.push_back(m_Collision.get());
	}
	
	// Start Setting
	{
		Vec2 start_Size = Vec2(225.f / 9.f, 45.f) * 3.5f;
		shared_ptr<ObImage> start = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_ManStart.png", start_Size, Int2(9, 1), true);
		start->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Start][EPlayerDir::Front] = start;
	}

	// Bat Idle
	{
		Vec2 bat_Size = Vec2(150.f / 3.f, 50.f) * 5.f;
		shared_ptr<ObImage> bat_Idle = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Bat_Idle.png", bat_Size, Int2(3, 1), true);
		bat_Idle->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front] = bat_Idle;
	}

	m_CurImg = m_Sprites[ECharacterState::Boss_Start][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}