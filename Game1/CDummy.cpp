#include "stdafx.h"
#include "CDummy.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"

CDummy::CDummy()
{
	m_Status->SetHp(100.f);

	Resource_Settings();
}

CDummy::~CDummy()
{
	Release();
}

void CDummy::BeginPlay()
{
	CCharacter::BeginPlay();


}

void CDummy::Tick()
{
	CCharacter::Tick();

#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("Dummy Hp : %f", m_Status->GetHp());
	}
	ImGui::End();
#endif


	// 위치 세팅
	Set_WorldPos(m_Collision->GetWorldPos());

	// 상태 또는 방향 변경 시 스프라이트 변경
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

void CDummy::LateUpdate()
{
	CCharacter::LateUpdate();

}

void CDummy::Render()
{
	CCharacter::Render();

	for (auto& col : m_vecCol)
		col->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& component : m_Components)
		component->Render();

}

void CDummy::Release()
{
	CCharacter::Release();

}

void CDummy::Update_Hit()
{
	IColorChanger::Update_Hit();

	if (m_HitTimer > 0.f)
	{
		m_HitTimer -= DELTA;

		if (m_HitTimer <= 0.f)
		{
			CHelper::OrignColor(Get_CurImage().get());
		}
	}
}

void CDummy::Hit_Color()
{
	IColorChanger::Hit_Color();

	CHelper::HitColor(Get_CurImage().get());
	m_HitTimer = 0.15f;
}

void CDummy::Hit_Effect()
{
	IColorChanger::Hit_Effect();


}

void CDummy::Resource_Settings()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f, 150.f);
	m_Collision->visible = false;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	Dummy_Image = RESOURCE->CreateImage(L"Object\\BossDummy.png", Vec2(29.f, 48.f), true);
	Dummy_Image->SetParentRT(*m_Collision);
	Dummy_Image->scale = Vec2(29.f, 48.f) * 3.0f;
	Dummy_Image->visible = true;

	m_Sprites[EPlayerState::Idle][EPlayerDir::Front] = Dummy_Image;
	
	// Start Image Setting
	m_CurImg = m_Sprites[EPlayerState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}

void CDummy::SetSprite(EPlayerState _State, EPlayerDir _Dir)
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

void CDummy::Play_Sprite(EPlayerState _State)
{
	CCharacter::Play_Sprite(_State);

	switch (_State)
	{
	case EPlayerState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		break;
	}
}
