#include "stdafx.h"
#include "CBlobRoller.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"

CBlobRoller::CBlobRoller()
{
	m_Status->SetHp(100.f);

	m_RenderLayer = ERenderLayer::Render_GameObject;

	Resource_Settings();
}

CBlobRoller::~CBlobRoller()
{
	Release();

}

void CBlobRoller::BeginPlay()
{
	CMonsterBase::BeginPlay();

	m_State->Change_State(ECharacterState::Attack1);
}

void CBlobRoller::Tick()
{
	CMonsterBase::Tick();


#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("BlobRoller Hp : %f", m_Status->GetHp());
	}
	ImGui::End();
#endif

	switch (m_State->Get_State())
	{
	case ECharacterState::Idle:		State_Idle();	 break;
	case ECharacterState::Attack1:	State_Attack();	 break;
	case ECharacterState::Hit:		State_Hit();	 break;

	case ECharacterState::PS_END: break;
	default: ;
	}

	// 위치 세팅
	cout << m_Pos.x << endl;

	if (m_prevState != m_State->Get_State() || m_prevDir != m_State->Get_Dir())
		SetSprite(m_State->Get_State(), m_State->Get_Dir());

	// Color 변경
	Update_Hit();

	for (auto& col : m_vecCol)
		col->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& component : m_Components)
		component->TickComponent();

}

void CBlobRoller::LateUpdate()
{
	CMonsterBase::LateUpdate();

}

void CBlobRoller::Render()
{
	CMonsterBase::Render();

	for (auto& col : m_vecCol)
		col->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& component : m_Components)
		component->Render();

}

void CBlobRoller::Release()
{
	CMonsterBase::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}
}

void CBlobRoller::State_Idle()
{

}

void CBlobRoller::State_Attack()
{

}

void CBlobRoller::State_Hit()
{

}

void CBlobRoller::Update_Hit()
{
	IColorChanger::Update_Hit();

	if (m_HitTimer > 0.f)
	{
		m_HitTimer -= DELTA;

		if (m_HitTimer <= 0.f)
		{
			CHelper::OrignColor(m_CurImg.get());
		}
	}
}

void CBlobRoller::Hit_Color()
{
	IColorChanger::Hit_Color();

	CHelper::HitColor(m_CurImg.get());
	m_HitTimer = 0.15f;
}

void CBlobRoller::Hit_Effect()
{
	IColorChanger::Hit_Effect();


}


void CBlobRoller::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	CMonsterBase::SetSprite(_State, _Dir);

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

void CBlobRoller::Play_Sprite(ECharacterState _State)
{
	CMonsterBase::Play_Sprite(_State);

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		break;

	case ECharacterState::Attack1:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;
	}
}


void CBlobRoller::Resource_Settings()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(70.f, 70.f);
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	// Idle
	{
		Vec2 idleSize = Vec2(32.f, 32.f) * 3.5f;
		shared_ptr<ObImage> idle = RESOURCE->CreateImage(L"Monster\\BlobRoller\\BlobRollerIdle.png", idleSize, true);
		idle->SetParentRT(*m_Collision);
		idle->visible = true;

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = idle;
	}
	// Attack
	{
		Vec2 attackSize = Vec2(224.f / 7.f, 32.f) * 3.5f;
		shared_ptr<ObImage> attack = RESOURCE->CreateSprite(L"Monster\\BlobRoller\\BlobRollerRoll.png", attackSize, Int2(7,1), true);
		attack->SetParentRT(*m_Collision);
		attack->visible = true;

		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front] = attack;
	}
	
	// Start Image Setting
	m_CurImg = m_Sprites[ECharacterState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}
