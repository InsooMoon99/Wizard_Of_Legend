#include "stdafx.h"
#include "CPlayer.h"

#include "CResourceManager.h"
#include "CInventoryComponent.h"
#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "CUIComponent.h"
#include "CStatusComponent.h"
#include "CCollisionManager.h"

CPlayer::CPlayer()
{
	m_State		= CHelper::CreateComponent<CStateComponent>(this);
	m_Weapon	= CHelper::CreateComponent<CWeaponComponent>(this);
	m_UI		= CHelper::CreateComponent<CUIComponent>(this);
	m_Inventory = CHelper::CreateComponent<CInventoryComponent>(this);

	m_Status->SetMaxHp(100.f);
	m_Status->SetMaxMp(100.f);

	m_Status->SetHp(80.f);
	m_Status->SetMp(60.f);
	m_Status->SetSpeed(500.f);

	m_Status->SetGold(100);

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

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	EPlayerState prevState = m_State->Get_State();
	EPlayerDir   prevDir = m_State->Get_Dir();

	Key_Input();

	if (m_State->Get_State() == EPlayerState::Idle || m_State->Get_State() == EPlayerState::Walk)
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

	case EPlayerState::Attack1:
	case EPlayerState::Attack2:
		State_Attack();
		break;
	}

	if (prevState != m_State->Get_State() || prevDir != m_State->Get_Dir())
	{
		SetSprite(m_State->Get_State(), m_State->Get_Dir());
	}

	// 마우스로 방향 조절 / 그런데 키보드로 하는게 나을듯
	{
		//m_AttackDir = INPUT->GetMouseWorldPos() - m_PlayerCol->GetWorldPos();
		//m_AttackDir.Normalize();
		//
		//float angle = atan2(m_AttackDir.y, m_AttackDir.x);
		//m_Player_Marker->rotation = angle - (PI / 2.0f);
	}

	// Update
	for (auto& Rect : m_vecRect)
		Rect->Update();

	for (auto& shadow : m_Shadow)
		shadow.second->Update();

	m_Player_Marker->Update();

	if (m_CurImg)
		m_CurImg->Update();

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

	for (auto& shadow : m_Shadow)
		shadow.second->Render();

	m_Player_Marker->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& dash : m_Dash)
		dash.second->Render();

	for (auto& component : m_Components)
		component->Render();

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
	EPlayerDir		curtDir = m_State->Get_Dir();
	EPlayerState	curState = m_State->Get_State();

	bool bCheck = m_State->Get_State() != EPlayerState::Attack1 && m_State->Get_State() != EPlayerState::Attack2;

	if (bCheck)
	{
		if (INPUT->KeyPress('W')) { m_MoveDir.y = +1.f; }
		if (INPUT->KeyPress('S')) { m_MoveDir.y = -1.f; }
		if (INPUT->KeyPress('A')) { m_MoveDir.x = -1.f; }
		if (INPUT->KeyPress('D')) { m_MoveDir.x = +1.f; }
	}

	if (INPUT->KeyDown('Q'))
	{
		m_State->Change_State(m_State->Next_AttackType());

		m_Weapon->Fire_Ball();

		return;
	}

	if (INPUT->KeyDown(VK_LBUTTON))
	{
		m_State->Change_State(m_State->Next_AttackType());

		return;
	}

	// 대각선 방향 우선 처리
	if ((m_MoveDir.x != 0) && (m_MoveDir.y != 0))
	{
		if (m_MoveDir.x > 0 && m_MoveDir.y > 0)
		{
			curtDir = EPlayerDir::Back_Right;
			m_Player_Marker->rotation = - (PI / 4.f);
			
		}
		else if (m_MoveDir.x < 0 && m_MoveDir.y > 0)
		{
			curtDir = EPlayerDir::Back_Left;
			m_Player_Marker->rotation = + (PI / 4.f);
		}
		else if (m_MoveDir.x < 0 && m_MoveDir.y < 0)
		{
			curtDir = EPlayerDir::Front_Left;
			m_Player_Marker->rotation = -PI - (PI / 4.f);
		}
		else if (m_MoveDir.x > 0 && m_MoveDir.y < 0)
		{
			curtDir = EPlayerDir::Front_Right;
			m_Player_Marker->rotation = -PI + (PI / 4.f);
		}
	}
	// 대각선이 아니면 상하좌우
	else
	{
		if (m_MoveDir.y > 0)
		{
			curtDir = EPlayerDir::Back;
			m_Player_Marker->rotation = 2 * PI;
		}
		else if (m_MoveDir.y < 0)
		{
			curtDir = EPlayerDir::Front;
			m_Player_Marker->rotation = -PI;
		}
		else if (m_MoveDir.x < 0)
		{
			curtDir = EPlayerDir::Left;
			m_Player_Marker->rotation = +(PI / 2);

		}
		else if (m_MoveDir.x > 0)
		{
			curtDir = EPlayerDir::Right;
			m_Player_Marker->rotation = -(PI / 2);
		}
	}

	m_State->Change_Dir(curtDir);
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

		m_Collision->MoveWorldPos(m_MoveDir * m_Status->GetSpeed() * DELTA);
	}
}

void CPlayer::State_Dash()
{
	if (m_CurImg->frame.x >= m_DashEndFrame)
		m_State->Change_State(EPlayerState::Idle);

	else
	{
		m_Collision->MoveWorldPos(m_LastDir * m_DashDistance * DELTA);

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
	// 재확인 코드
	auto& map = m_Sprites[m_State->Get_State()];
	auto it = map.find(m_State->Get_Dir());
	if (it != map.end() && m_CurImg != it->second)
		return;

	if (m_CurImg && m_CurImg->frame.x >= m_CurImg->maxFrame.x - 1)
		m_State->Change_State(EPlayerState::Idle);
}

void CPlayer::SetSprite(EPlayerState _State, EPlayerDir _Dir)
{
	CCharacter::SetSprite(_State, _Dir);

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

		Play_Sprite(_State);
	}

	Shadow_Size();
}

void CPlayer::Play_Sprite(EPlayerState _State)
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
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.03f);
		m_CurImg->frame = Int2(0, 0);
		m_DashEndFrame = m_CurImg->maxFrame.x - 1;

		Dash_Sprite();
		break;

	case EPlayerState::Attack1:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.06f);
		m_CurImg->frame = Int2(0, 0);
		break;

	case EPlayerState::Attack2:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.06f);
		m_CurImg->frame = Int2(0, 0);
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

void CPlayer::Dash_Sprite()
{
	// 대쉬 이펙트
	auto iter = m_Dash.find(m_State->Get_Dir());
	if (iter != m_Dash.end())
	{
		shared_ptr<ObImage> dashEffect = iter->second;
		dashEffect->SetWorldPos(m_Collision->GetWorldPos());
		dashEffect->visible = true;
		dashEffect->ChangeAnim(ANIMSTATE::ONCE, 0.03f);
		dashEffect->frame = Int2(0, 0);
	}

}

void CPlayer::Resource_Settings()
{
	// Player Collision
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f, 100.f);
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->pivot = OFFSET_N;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	m_vecRect.push_back(m_Collision.get());

	// State_Idle
	{
		Vec2 idle_Size = Vec2(90.f, 93.f) * 1.6f;
		shared_ptr<ObImage> front_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),true);
		front_Idle->SetParentRT(*m_Collision);
		front_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		shared_ptr<ObImage> front_Left_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),true);
		front_Left_Idle->SetParentRT(*m_Collision);
		front_Left_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		shared_ptr<ObImage> front_Right_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),true);
		front_Right_Idle->SetParentRT(*m_Collision);
		front_Right_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		shared_ptr<ObImage> back_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),false);
		back_Idle->SetParentRT(*m_Collision);
		back_Idle->SetLocalPos(Vec2(1.5f, 10.f));
		back_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> back_Left_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),false);
		back_Left_Idle->SetParentRT(*m_Collision);
		back_Left_Idle->SetLocalPos(Vec2(1.5f, 10.f));
		back_Left_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> back_Right_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),false);
		back_Right_Idle->SetParentRT(*m_Collision);
		back_Right_Idle->SetLocalPos(Vec2(1.5f, 10.f));
		back_Right_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> left_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),false);
		left_Idle->SetParentRT(*m_Collision);
		left_Idle->SetLocalPos(Vec2(-15.f, 10.f));
		left_Idle->frame = Int2(2, 1);

		shared_ptr<ObImage> right_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png",idle_Size,Int2(4, 1),false);
		right_Idle->SetParentRT(*m_Collision);
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

		shared_ptr<ObImage> front_Walk = RESOURCE->CreateSprite(L"Player\\Front\\Front_Walk.png", walkSize,Int2(10, 1), false);
		front_Walk->SetParentRT(*m_Collision);
		front_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);

		m_Sprites[EPlayerState::Walk][EPlayerDir::Front] = front_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Front_Left] = front_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Front_Right] = front_Walk;

		shared_ptr<ObImage> back_Walk = RESOURCE->CreateSprite(L"Player\\Back\\Back_Walk.png", walkSize,Int2(10, 1), false);
		back_Walk->SetParentRT(*m_Collision);
		back_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);

		m_Sprites[EPlayerState::Walk][EPlayerDir::Back] = back_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Back_Left] = back_Walk;
		m_Sprites[EPlayerState::Walk][EPlayerDir::Back_Right] = back_Walk;

		shared_ptr<ObImage> left_Walk = RESOURCE->CreateSprite(L"Player\\Left\\Left_Walk.png", walkSize,Int2(10, 1), false);
		left_Walk->SetParentRT(*m_Collision);
		left_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[EPlayerState::Walk][EPlayerDir::Left] = left_Walk;

		shared_ptr<ObImage> right_Walk = RESOURCE->CreateSprite(L"Player\\Right\\Right_Walk.png", walkSize,Int2(10, 1), false);
		right_Walk->SetParentRT(*m_Collision);
		right_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[EPlayerState::Walk][EPlayerDir::Right] = right_Walk;
	}

	// Shadow
	{
		shared_ptr<ObImage> shadow_Idle = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f);
		shadow_Idle->SetParentRT(*m_Collision);
		shadow_Idle->SetLocalPos(Vec2(-2.5f, -40.f));
		shadow_Idle->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		shadow_Idle->visible = true;
		m_Shadow.insert({ EPlayerState::Idle, shadow_Idle });

		shared_ptr<ObImage> shadow_Attack = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(80.f, 80.f) * 1.4f);
		shadow_Attack->SetParentRT(*m_Collision);
		shadow_Attack->SetLocalPos(Vec2(0.0f, -40.f));
		shadow_Attack->color = Color(0.0f, 0.0f, 0.0f, 0.15f);
		m_Shadow.insert({ EPlayerState::Attack1, shadow_Attack });
		m_Shadow.insert({ EPlayerState::Attack2, shadow_Attack });

		shared_ptr<ObImage> shadow_Walk = RESOURCE->CreateImage(L"Shadow\\MIDDLE_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f);
		shadow_Walk->SetParentRT(*m_Collision);
		shadow_Walk->SetLocalPos(Vec2(-2.5f, -40.f));
		shadow_Walk->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		shadow_Walk->visible = true;
		m_Shadow.insert({ EPlayerState::Walk, shadow_Walk });

		m_StartShadowScale = Vec2(60.f, 60.f) * 1.4f;
		m_EndShadowScale = Vec2(80.f, 60.f) * 1.6f;

		shared_ptr<ObImage> shadow_Dash = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f, false);
		shadow_Dash->SetParentRT(*m_Collision);
		shadow_Dash->SetLocalPos(Vec2(-2.5f, -40.f));
		shadow_Dash->color = Color(0.0f, 0.0f, 0.0f, 0.25f);

		m_Shadow.insert({ EPlayerState::Dash, shadow_Dash });
	}

	// State_Dash
	{
		Vec2 dash_Size = Vec2(1100.f / 11, 104.f) * 1.5f;

		shared_ptr<ObImage> front_Dash = RESOURCE->CreateSprite(L"Player\\front\\Front_Dash_long.png",dash_Size,Int2(11, 1), false);
		front_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> front_Left_Dash = RESOURCE->CreateSprite(L"Player\\front\\Front_Dash_long.png",dash_Size,Int2(11, 1), false);
		front_Left_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> front_Right_Dash = RESOURCE->CreateSprite(L"Player\\front\\Front_Dash_long.png",dash_Size,Int2(11, 1), false);
		front_Right_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> back_Dash = RESOURCE->CreateSprite(L"Player\\Back\\Back_Dash_long.png",dash_Size,Int2(11, 1), false);
		back_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> back_Left_Dash = RESOURCE->CreateSprite(L"Player\\Back\\Back_Dash_long.png",dash_Size,Int2(11, 1), false);
		back_Left_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> back_Right_Dash = RESOURCE->CreateSprite(L"Player\\Back\\Back_Dash_long.png",dash_Size,Int2(11, 1), false);
		back_Right_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> left_Dash = RESOURCE->CreateSprite(L"Player\\Left\\Left_Dash_long.png",dash_Size,Int2(11, 1), false);
		left_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> right_Dash = RESOURCE->CreateSprite(L"Player\\Right\\Right_Dash_long.png",dash_Size,Int2(11, 1), false);
		right_Dash->SetParentRT(*m_Collision);
		right_Dash->SetLocalPosX(-10.f);

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
		float dash_Alpha = 0.3f;

		shared_ptr<ObImage> dash_Front_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png",dash_Effect_Size,Int2(11, 1), false);
		dash_Front_Effect->rotation = -(PI / 2.f);
		dash_Front_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Front, dash_Front_Effect });

		// 좌하단
		shared_ptr<ObImage> dash_Front_Left_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png",dash_Effect_Size,Int2(11, 1), false);
		dash_Front_Left_Effect->rotation = -3.f * PI / 4.f;
		dash_Front_Left_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Front_Left, dash_Front_Left_Effect });

		// 우하단
		shared_ptr<ObImage> dash_Front_Right_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png",dash_Effect_Size,Int2(11, 1), false);
		dash_Front_Right_Effect->rotation = -(PI / 4.f);
		dash_Front_Right_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Front_Right, dash_Front_Right_Effect });

		shared_ptr<ObImage> dash_Back_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", dash_Effect_Size,Int2(11, 1), false);
		dash_Back_Effect->rotation = PI / 2.f;
		dash_Back_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Back, dash_Back_Effect });

		// 좌상단
		shared_ptr<ObImage> dash_Back_Left_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png",dash_Effect_Size,Int2(11, 1), false);
		dash_Back_Left_Effect->rotation = 3.f * PI / 4.f;
		dash_Back_Left_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Back_Left, dash_Back_Left_Effect });

		// 우상단
		shared_ptr<ObImage> dash_Back_Right_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png",dash_Effect_Size,Int2(11, 1), false);
		dash_Back_Right_Effect->rotation = PI / 4.f;
		dash_Back_Right_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Back_Right, dash_Back_Right_Effect });

		shared_ptr<ObImage> dash_Right_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", Vec2(220.f / 11.f, 60.f) * 3.5f,Int2(11, 1), false);
		dash_Right_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Right, dash_Right_Effect });

		shared_ptr<ObImage> dash_Left_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png",Vec2(220.f / 11.f, 60.f) * 3.5f,Int2(11, 1), false);
		dash_Left_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Left, dash_Left_Effect });
	}

	// Attack
	{
		// Front
		Vec2 attack_Size{ (720.f / 8.f, 93.f) * 1.4f };

		shared_ptr<ObImage> attack_Front1 = RESOURCE->CreateSprite(L"Player\\Front\\Front_Left_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Front1->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Front] = attack_Front1;
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Front_Left] = attack_Front1;
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Front_Right] = attack_Front1;


		shared_ptr<ObImage> attack_Front2 = RESOURCE->CreateSprite(L"Player\\Front\\Front_Left_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Front2->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Front] = attack_Front2;
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Front_Left] = attack_Front2;
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Front_Right] = attack_Front2;

		// Back
		shared_ptr<ObImage> attack_Back1 = RESOURCE->CreateSprite(L"Player\\Back\\Back_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Back1->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Back] = attack_Back1;

		shared_ptr<ObImage> attack_Back2 = RESOURCE->CreateSprite(L"Player\\Back\\Back_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Back2->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Back] = attack_Back2;
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Back_Left] = attack_Back2;
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Back_Right] = attack_Back2;

		// Left
		shared_ptr<ObImage> attack_Left1 = RESOURCE->CreateSprite(L"Player\\Left\\Left_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Left1->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Left] = attack_Left1;
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Back_Left] = attack_Left1;


		shared_ptr<ObImage> attack_Left2 = RESOURCE->CreateSprite(L"Player\\Left\\Left_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Left2->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Left] = attack_Left2;
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Back_Left] = attack_Left2;

		// Right
		auto attack_Right1 = RESOURCE->CreateSprite(L"Player\\Right\\Right_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Right1->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Right] = attack_Right1;
		m_Sprites[EPlayerState::Attack1][EPlayerDir::Back_Right] = attack_Right1;

		auto attack_Right2 = RESOURCE->CreateSprite(L"Player\\Right\\Right_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Right2->SetParentRT(*m_Collision);
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Right] = attack_Right2;
		m_Sprites[EPlayerState::Attack2][EPlayerDir::Back_Right] = attack_Right2;

	}

	// Direction Overlay
	{
		// TODO : Glory 도 추가하기?
		m_Player_Marker = RESOURCE->CreateImage(L"UI\\Player_Marker.png", Vec2(32.f, 33.f) * 3.f);
		m_Player_Marker->SetParentRT(*m_Collision);
		m_Player_Marker->SetLocalPos(Vec2(-2.f, -35.f));
		m_Player_Marker->rotation = -PI;
		m_Player_Marker->color.A(0.15f);
		m_Player_Marker->visible = true;
	}

	// Start Image Setting
	m_CurImg = m_Sprites[EPlayerState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}

void CPlayer::Print_ImGui()
{
	if (ImGui::Begin("Player"))
	{
		float hp = m_Status->GetHp();
		float mp = m_Status->GetMp();
		int gold = m_Status->GetGold();
		float speed = m_Status->GetSpeed();

		ImGui::Text("MaxHp", m_Status->GetMaxHp());
		ImGui::InputFloat("Hp", &hp);
		m_Status->SetHp(hp);

		ImGui::InputFloat("Mp", &mp);
		m_Status->SetMp(mp);

		ImGui::InputFloat("Speed", &speed);
		m_Status->SetSpeed(speed);

		ImGui::InputInt("Gold", &gold);
		m_Status->SetGold(gold);

		ImGui::Text("State : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
		ImGui::Text("Dir   : %s", CHelper::ToString_PlayerDir(m_State->Get_Dir()));
		ImGui::Text("LastDir.x : %f", m_LastDir.x);
		ImGui::Text("LastDir.y : %f", m_LastDir.y);

		if (m_CurImg)
		{
			Vec2 localPos = m_CurImg->GetLocalPos();
			ImGui::InputFloat2("Sprite Pos", &localPos.x);
			m_CurImg->SetLocalPos(localPos);
		}

		// 위치 세팅
		Vec2 worldPos = m_Collision->GetWorldPos();
		if (ImGui::InputFloat2("Player Pos", &worldPos.x))
		{
			m_Collision->SetWorldPos(worldPos);
		}
	}

	ImGui::End();
}
