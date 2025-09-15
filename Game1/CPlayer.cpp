#include "stdafx.h"
#include "CPlayer.h"

#include "CResourceManager.h"
#include "CInventoryComponent.h"
#include "CItemComponent.h"
#include "CStateComponent.h"
#include "CWeaponComponent.h"

CPlayer::CPlayer()
{
	m_State = CHelper::CreateComponent<CStateComponent>(this);
	m_Weapon = CHelper::CreateComponent<CWeaponComponent>(this);
	m_Inventory = CHelper::CreateComponent<CInventoryComponent>(this);
	m_Item = CHelper::CreateComponent<CItemComponent>(this);

	m_Hp = 100.f;
	m_Speed = 500.f;

	// 컴포넌트 세팅하고 호출해야함
	Resource_Settings();
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::BeginPlay()
{
	CActor::BeginPlay();


}

void CPlayer::Tick()
{
	CActor::Tick();

	Print_ImGui();

	EPlayerState prevState = m_State->Get_State();
	EPlayerDir   prevDir = m_State->Get_Dir();

	Key_Input();

	if (m_State->Get_State() != EPlayerState::Dash)
	{
		if (INPUT->KeyDown(VK_SPACE))
		{
			m_State->Change_State(EPlayerState::Dash);
		}
		else if (m_MoveDir.x != 0 || m_MoveDir.y != 0)
		{
			m_State->Change_State(EPlayerState::Walk);

			m_LastDir = m_MoveDir;
			m_LastDir.Normalize();
		}
		else
		{
			m_State->Change_State(EPlayerState::Idle);
		}
	}

	switch (m_State->Get_State())
	{
	case EPlayerState::Idle:
		State_Idle();
		break;

	case EPlayerState::Walk:
		State_Walk();
		break;

	case EPlayerState::Dash:
		State_Dash();
		break;

	case EPlayerState::Attack:
		State_Attack();
		break;
	}

	if (prevState != m_State->Get_State() || prevDir != m_State->Get_Dir())
	{
		SetSprite(m_State->Get_State(), m_State->Get_Dir());
	}

	// Update
	for (auto& Rect : m_vecRect)
		Rect->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& shadow : m_Shadow)
		shadow.second->Update();

	for (auto& dash : m_Dash)
		dash.second->Update();

	for (auto& component : m_Components)
		component->TickComponent();

	
}


void CPlayer::LateUpdate()
{
	CActor::LateUpdate();
}

void CPlayer::Render()
{
	CActor::Render();

	for (auto& Rect : m_vecRect)
		Rect->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& shadow : m_Shadow)
		shadow.second->Render();

	for (auto& dash : m_Dash)
		dash.second->Render();

	// Component Render ? 지금은 필요가 없다.

}

void CPlayer::Release()
{
	CActor::Release();

	m_Sprites.clear();
	m_Shadow.clear();
	m_Dash.clear();

	m_CurImg.reset();

	for (auto& component : m_Components)
		SafeDelete(component);

	for (auto Rect : m_vecRect)
		SafeDelete(Rect);
}

void CPlayer::Key_Input()
{
	if (m_State->Get_State() == EPlayerState::Dash)
		return;

	m_MoveDir = Vec2(0.f, 0.f);
	EPlayerDir currentDir = m_State->Get_Dir();

	if (INPUT->KeyPress('W')) { m_MoveDir.y = +1.f; }
	if (INPUT->KeyPress('S')) { m_MoveDir.y = -1.f; }
	if (INPUT->KeyPress('A')) { m_MoveDir.x = -1.f; }
	if (INPUT->KeyPress('D')) { m_MoveDir.x = +1.f; }

	// 대각선 방향 우선 처리
	if ((m_MoveDir.x != 0) && (m_MoveDir.y != 0))
	{
		if (m_MoveDir.x > 0 && m_MoveDir.y > 0)
		{
			currentDir = EPlayerDir::Back_Right;
		}
		else if (m_MoveDir.x < 0 && m_MoveDir.y > 0)
		{
			currentDir = EPlayerDir::Back_Left;
		}
		else if (m_MoveDir.x < 0 && m_MoveDir.y < 0)
		{
			currentDir = EPlayerDir::Front_Left;
		}
		else if (m_MoveDir.x > 0 && m_MoveDir.y < 0)
		{
			currentDir = EPlayerDir::Front_Right;
		}
	}
	// 대각선이 아니면 상하좌우
	else
	{
		if (m_MoveDir.y > 0)
		{
			currentDir = EPlayerDir::Back;
		}
		else if (m_MoveDir.y < 0)
		{
			currentDir = EPlayerDir::Front;
		}
		else if (m_MoveDir.x < 0)
		{
			currentDir = EPlayerDir::Left;
		}
		else if (m_MoveDir.x > 0)
		{
			currentDir = EPlayerDir::Right;
		}
	}

	m_State->Change_Dir(currentDir);
}

void CPlayer::State_Idle()
{
	// TODO : Idle 상태를 따로 처리해야하는가?
	//m_PlayerCol->MoveWorldPos(m_MoveDir * m_Speed * DELTA);
}

void CPlayer::State_Walk()
{
	if (m_MoveDir.Length() > 0.f)
	{
		m_MoveDir.Normalize();

		m_PlayerCol->MoveWorldPos(m_MoveDir * m_Speed * DELTA);
	}
}

void CPlayer::State_Dash()
{
	if (m_CurImg->frame.x >= m_DashEndFrame)
		m_State->Change_State(EPlayerState::Idle);

	else
	{
		m_PlayerCol->MoveWorldPos(m_LastDir * m_DashDistance * DELTA);

		// 그림자 크기 조절
		{
			if (m_CurImg->frame.x < 5)
			{
				float lerpTime = (float)m_CurImg->frame.x / 5.0f;
				m_Shadow[EPlayerState::Dash]->scale = Vec2::Lerp(m_StartShadowScale, m_EndShadowScale, lerpTime);
			}
			else if (m_CurImg->frame.x < m_DashEndFrame)
			{
				float lerpTime = (float)(m_CurImg->frame.x - 5) / (float)(m_DashEndFrame - 5);
				m_Shadow[EPlayerState::Dash]->scale = Vec2::Lerp(m_EndShadowScale, m_StartShadowScale, lerpTime);
			}
		}

	}
}

void CPlayer::State_Attack()
{

}

void CPlayer::SetSprite(EPlayerState _State, EPlayerDir _Dir)
{
	if (m_CurImg)
		m_CurImg->visible = false;

	for (auto& shadow : m_Shadow)
		shadow.second->visible = false;

	for (auto& dash : m_Dash)
		dash.second->visible = false;

	auto& stateIter = m_Sprites[_State];
	auto imgIter = stateIter.find(_Dir);

	if (imgIter != stateIter.end())
	{
		m_CurImg = imgIter->second;
		m_CurImg->visible = true;

		State_Sprite(_State);
	}

	Shadow_Size();

}

void CPlayer::State_Sprite(EPlayerState _State)
{
	switch (_State)
	{
	case EPlayerState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		break;

	case EPlayerState::Walk:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case EPlayerState::Dash:
		// Dash 스프라이트
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.03f);
		m_CurImg->frame = Int2(0, 0);
		m_DashEndFrame = m_CurImg->maxFrame.x - 1;

		Dash_Effect();

		break;

	case EPlayerState::Attack:
		// TODO : 공격 추가

		break;
	}
}

void CPlayer::Shadow_Size()
{
	// Shadow
	if (m_Shadow.find(m_State->Get_State()) != m_Shadow.end())
	{
		if (m_State->Get_State() == EPlayerState::Dash)
		{
			m_Shadow[EPlayerState::Dash]->scale = m_StartShadowScale;
		}

		m_Shadow[m_State->Get_State()]->visible = true;

	}
}

void CPlayer::Dash_Effect()
{
	// 대쉬 이펙트
	auto iter = m_Dash.find(m_State->Get_Dir());
	if (iter != m_Dash.end())
	{
		shared_ptr<ObImage> dashEffect = iter->second;
		dashEffect->SetWorldPos(m_PlayerCol->GetWorldPos());
		dashEffect->visible = true;
		dashEffect->ChangeAnim(ANIMSTATE::ONCE, 0.03f);
		dashEffect->frame = Int2(0, 0);
	}

}

void CPlayer::Resource_Settings()
{
	// Player Collision
	m_PlayerCol = make_unique<ObRect>();
	m_PlayerCol->scale = Vec2(100.f, 100.f);
	m_PlayerCol->visible = true;
	m_PlayerCol->isFilled = false;
	m_PlayerCol->pivot = OFFSET_N;
	m_PlayerCol->collider = COLLIDER::RECT;

	m_vecRect.push_back(m_PlayerCol.get());

	// State_Idle
	{
		Vec2 idle_Size = Vec2(90.f, 93.f) * 1.6f;
		shared_ptr<ObImage> front_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			true);
		front_Idle->SetParentRT(*m_PlayerCol);
		front_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		shared_ptr<ObImage> front_Left_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			true);
		front_Left_Idle->SetParentRT(*m_PlayerCol);
		front_Left_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		shared_ptr<ObImage> front_Right_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			true);
		front_Right_Idle->SetParentRT(*m_PlayerCol);
		front_Right_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		shared_ptr<ObImage> back_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		back_Idle->SetParentRT(*m_PlayerCol);
		back_Idle->SetLocalPos(Vec2(1.5f, 10.f));
		back_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> back_Left_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		back_Left_Idle->SetParentRT(*m_PlayerCol);
		back_Left_Idle->SetLocalPos(Vec2(1.5f, 10.f));
		back_Left_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> back_Right_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		back_Right_Idle->SetParentRT(*m_PlayerCol);
		back_Right_Idle->SetLocalPos(Vec2(1.5f, 10.f));
		back_Right_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> left_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		left_Idle->SetParentRT(*m_PlayerCol);
		left_Idle->SetLocalPos(Vec2(-15.f, 10.f));
		left_Idle->frame = Int2(2, 1);

		shared_ptr<ObImage> right_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		right_Idle->SetParentRT(*m_PlayerCol);
		right_Idle->SetLocalPos(Vec2(-21.f, 10.f));
		right_Idle->frame = Int2(3, 1);

		m_Sprites[EPlayerState::Idle][EPlayerDir::Front]		= front_Idle;
		m_Sprites[EPlayerState::Idle][EPlayerDir::Front_Left]	= front_Left_Idle;
		m_Sprites[EPlayerState::Idle][EPlayerDir::Front_Right]	= front_Right_Idle;
		m_Sprites[EPlayerState::Idle][EPlayerDir::Back]			= back_Idle;
		m_Sprites[EPlayerState::Idle][EPlayerDir::Back_Left]	= back_Left_Idle;
		m_Sprites[EPlayerState::Idle][EPlayerDir::Back_Right]	= back_Right_Idle;
		m_Sprites[EPlayerState::Idle][EPlayerDir::Left]			= left_Idle;
		m_Sprites[EPlayerState::Idle][EPlayerDir::Right]		= right_Idle;
	}

	// State_Walk
	{
		Vec2 walkSize = Vec2(900.f / 10.f, 93.f) * 1.4f;

		shared_ptr<ObImage> front_Walk = RESOURCE->CreateSprite(
			L"Player\\Front\\Front_Walk.png", walkSize,
			Int2(10, 1), false);

		front_Walk->SetParentRT(*m_PlayerCol);
		front_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);

		m_Sprites[EPlayerState::Walk][EPlayerDir::Front] = front_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Front_Left] = front_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Front_Right] = front_Walk;

		shared_ptr<ObImage> back_Walk = RESOURCE->CreateSprite(
			L"Player\\Back\\Back_Walk.png", walkSize,
			Int2(10, 1), false);
		back_Walk->SetParentRT(*m_PlayerCol);
		back_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);

		m_Sprites[EPlayerState::Walk][EPlayerDir::Back] = back_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Back_Left] = back_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Back_Right] = back_Walk;

		shared_ptr<ObImage> left_Walk = RESOURCE->CreateSprite(
			L"Player\\Left\\Left_Walk.png", walkSize,
			Int2(10, 1), false);
		left_Walk->SetParentRT(*m_PlayerCol);
		left_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[EPlayerState::Walk][EPlayerDir::Left] = left_Walk;

		shared_ptr<ObImage> right_Walk = RESOURCE->CreateSprite(
			L"Player\\Right\\Right_Walk.png", walkSize,
			Int2(10, 1), false);
		right_Walk->SetParentRT(*m_PlayerCol);
		right_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[EPlayerState::Walk][EPlayerDir::Right] = right_Walk;
	}

	// Shadow
	{
		shared_ptr<ObImage> shadow_Idle = RESOURCE->CreateImage(
			L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f);

		shadow_Idle->SetParentRT(*m_PlayerCol);
		shadow_Idle->SetLocalPos(Vec2(-2.5f, -40.f));
		shadow_Idle->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		shadow_Idle->visible = true;
		m_Shadow.insert({ EPlayerState::Idle, shadow_Idle });

		shared_ptr<ObImage> shadow_Walk = RESOURCE->CreateImage(
			L"Shadow\\MIDDLE_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f);

		shadow_Walk->SetParentRT(*m_PlayerCol);
		shadow_Walk->SetLocalPos(Vec2(-2.5f, -40.f));
		shadow_Walk->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		shadow_Walk->visible = true;
		m_Shadow.insert({ EPlayerState::Walk, shadow_Walk });

		m_StartShadowScale = Vec2(60.f, 60.f) * 1.2f;
		m_EndShadowScale = Vec2(100.f, 60.f) * 1.8f;

		shared_ptr<ObImage> shadow_Dash = RESOURCE->CreateImage(
			L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f, false);

		shadow_Dash->SetParentRT(*m_PlayerCol);
		shadow_Dash->SetLocalPos(Vec2(-2.5f, -40.f));
		shadow_Dash->color = Color(0.0f, 0.0f, 0.0f, 0.25f);

		m_Shadow.insert({ EPlayerState::Dash, shadow_Dash });
	}

	// State_Dash
	{
		Vec2 dash_Size = Vec2(1100.f / 11, 104.f) * 1.5f;

		shared_ptr<ObImage> front_Dash = RESOURCE->CreateSprite(
			L"Player\\front\\Front_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		front_Dash->SetParentRT(*m_PlayerCol);

		shared_ptr<ObImage> front_Left_Dash = RESOURCE->CreateSprite(
			L"Player\\front\\Front_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		front_Left_Dash->SetParentRT(*m_PlayerCol);

		shared_ptr<ObImage> front_Right_Dash = RESOURCE->CreateSprite(
			L"Player\\front\\Front_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		front_Right_Dash->SetParentRT(*m_PlayerCol);

		shared_ptr<ObImage> back_Dash = RESOURCE->CreateSprite(
			L"Player\\Back\\Back_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		back_Dash->SetParentRT(*m_PlayerCol);

		shared_ptr<ObImage> back_Left_Dash = RESOURCE->CreateSprite(
			L"Player\\Back\\Back_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		back_Left_Dash->SetParentRT(*m_PlayerCol);

		shared_ptr<ObImage> back_Right_Dash = RESOURCE->CreateSprite(
			L"Player\\Back\\Back_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		back_Right_Dash->SetParentRT(*m_PlayerCol);

		shared_ptr<ObImage> left_Dash = RESOURCE->CreateSprite(
			L"Player\\Left\\Left_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		left_Dash->SetParentRT(*m_PlayerCol);

		shared_ptr<ObImage> right_Dash = RESOURCE->CreateSprite(
			L"Player\\Right\\Right_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		right_Dash->SetParentRT(*m_PlayerCol);

		m_Sprites[EPlayerState::Dash][EPlayerDir::Front] = front_Dash;
		m_Sprites[EPlayerState::Dash][EPlayerDir::Front_Left] = front_Left_Dash;
		m_Sprites[EPlayerState::Dash][EPlayerDir::Front_Right] = front_Right_Dash;

		m_Sprites[EPlayerState::Dash][EPlayerDir::Back] = back_Dash;
		m_Sprites[EPlayerState::Dash][EPlayerDir::Back_Left] = back_Left_Dash;
		m_Sprites[EPlayerState::Dash][EPlayerDir::Back_Right] = back_Right_Dash;

		m_Sprites[EPlayerState::Dash][EPlayerDir::Left] = left_Dash;
		m_Sprites[EPlayerState::Dash][EPlayerDir::Right] = right_Dash;



	}

	// Dash_Effect
	{
		Vec2 dash_Effect_Size = Vec2(220.f / 11.f, 60.f) * 3.5f;

		shared_ptr<ObImage> dash_Front_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png",
			dash_Effect_Size,
			Int2(11, 1), false);

		dash_Front_Effect->rotation = -(PI / 2.f);
		dash_Front_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Front, dash_Front_Effect });

		// 좌하단
		shared_ptr<ObImage> dash_Front_Left_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png",
			dash_Effect_Size,
			Int2(11, 1), false);
		dash_Front_Left_Effect->rotation = -3.f * PI / 4.f;
		dash_Front_Left_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Front_Left, dash_Front_Left_Effect });

		// 우하단
		shared_ptr<ObImage> dash_Front_Right_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png",
			dash_Effect_Size,
			Int2(11, 1), false);
		dash_Front_Right_Effect->rotation = -(PI / 4.f);
		dash_Front_Right_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Front_Right, dash_Front_Right_Effect });

		shared_ptr<ObImage> dash_Back_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png",
			Vec2(220.f / 11.f, 60.f) * 3.5f,
			Int2(11, 1), false);

		dash_Back_Effect->rotation = PI / 2.f;
		dash_Back_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Back, dash_Back_Effect });

		// 좌상단
		shared_ptr<ObImage> dash_Back_Left_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png",
			dash_Effect_Size,
			Int2(11, 1), false);
		dash_Back_Left_Effect->rotation = 3.f * PI / 4.f;
		dash_Back_Left_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Back_Left, dash_Back_Left_Effect });

		// 우상단
		shared_ptr<ObImage> dash_Back_Right_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png",
			dash_Effect_Size,
			Int2(11, 1), false);
		dash_Back_Right_Effect->rotation = PI / 4.f;
		dash_Back_Right_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Back_Right, dash_Back_Right_Effect });

		shared_ptr<ObImage> dash_Right_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png", 
			Vec2(220.f / 11.f, 60.f) * 3.5f,
			Int2(11, 1), false);

		dash_Right_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Right, dash_Right_Effect });

		shared_ptr<ObImage> dash_Left_Effect = RESOURCE->CreateSprite(
			L"Effect\\Dash_Effect.png",
			Vec2(220.f / 11.f, 60.f) * 3.5f,
			Int2(11, 1), false);

		dash_Left_Effect->color.A(0.5f);
		m_Dash.insert({ EPlayerDir::Left, dash_Left_Effect });
	}

	// Start Image Setting
	m_CurImg = m_Sprites[EPlayerState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}

void CPlayer::Print_ImGui()
{
	if (ImGui::Begin("Player"))
	{
		ImGui::Text("Hp    : %.2f", m_Hp);
		ImGui::Text("Speed : %.2f", m_Speed);
		ImGui::Text("State : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
		ImGui::Text("Dir   : %s", CHelper::ToString_PlayerDir(m_State->Get_Dir()));
		ImGui::Text("LastDir.x : %f", m_LastDir.x);
		ImGui::Text("LastDir.y : %f", m_LastDir.y);

		if (m_CurImg)
		{
			Vec2 localPos = m_CurImg->GetLocalPos();
			ImGui::InputFloat2("Sprite Local Position", &localPos.x);
			m_CurImg->SetLocalPos(localPos);
		}

		// 위치 세팅
		Vec2 worldPos = m_PlayerCol->GetWorldPos();
		if (ImGui::InputFloat2("Player Pos", &worldPos.x))
		{
			m_PlayerCol->SetWorldPos(worldPos);
		}

	}

	ImGui::End();
}
