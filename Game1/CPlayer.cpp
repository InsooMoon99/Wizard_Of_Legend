#include "stdafx.h"
#include "CPlayer.h"
#include "CResourceManager.h"

CPlayer::CPlayer()
{
	Resource_Settings();

	m_Hp = 100.f;
	m_Speed = 100.f;
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::BeginPlay()
{


}

void CPlayer::Tick()
{
	Print_ImGui();

	// 이전 상태를 저장
	PlayerState prevState = m_PlayerState;
	PlayerDir prevDir = m_PlayerDir;

	// 대시 상태가 아닐 때만 키 입력 처리
	if (m_PlayerState != PlayerState::Dash)
	{
		m_MoveDir = Vec2(0.f, 0.f);

		if (INPUT->KeyPress('W')) { m_MoveDir.y = 1.f; m_PlayerDir = PlayerDir::Back; }
		else if (INPUT->KeyPress('S')) { m_MoveDir.y = -1.f; m_PlayerDir = PlayerDir::Front; }

		if (INPUT->KeyPress('A')) { m_MoveDir.x = -1.f; m_PlayerDir = PlayerDir::Left; }
		else if (INPUT->KeyPress('D')) { m_MoveDir.x = 1.f; m_PlayerDir = PlayerDir::Right; }

		if (m_MoveDir.x != 0 || m_MoveDir.y != 0)
		{
			m_PlayerState = PlayerState::Walk;
			m_LastDir = m_MoveDir;
			m_LastDir.Normalize();
		}
		else
		{
			m_PlayerState = PlayerState::Idle;
		}

		if (INPUT->KeyDown(VK_SPACE))
		{
			m_PlayerState = PlayerState::Dash;
		}
	}

	// 상태에 따른 행동 실행
	if (m_PlayerState == PlayerState::Dash)
	{
		// 대시 애니메이션이 끝나면 Idle 상태로 돌아감
		if (m_CurImg->frame.x >= m_DashEndFrame)
		{
			m_PlayerState = PlayerState::Idle;
		}
		else
		{
			// 대시 애니메이션이 진행 중일 때만 이동
			m_PlayerCol->MoveWorldPos(m_LastDir * m_DashDistance * DELTA);
		}
	}
	else
	{
		m_PlayerCol->MoveWorldPos(m_MoveDir * 400.f * DELTA);
	}

	// 상태나 방향이 변경되었을 때만 스프라이트 업데이트
	if (prevState != m_PlayerState || prevDir != m_PlayerDir)
	{
		SetSprite(m_PlayerState, m_PlayerDir);
	}

	// Update
	for (auto& Rect : m_vecRect)
		Rect->Update();

	for (auto& shadow : m_Shadow)
		shadow.second->Update();

	for (auto& [state, dirImage] : m_Sprites)
	{
		for (auto& [dir, sprite] : dirImage)
		{
			if (sprite)
				sprite->Update();
		}
	}
}


void CPlayer::LateUpdate()
{

}

void CPlayer::Render()
{
	for (auto& Rect : m_vecRect)
		Rect->Render();

	for (auto& shadow : m_Shadow)
		shadow.second->Render();

	for (auto& [state, dirImage] : m_Sprites)
	{
		for (auto& [dir, sprite] : dirImage)
		{
			if (sprite)
				sprite->Render();
		}
	}

}

void CPlayer::Release()
{
	for (auto& [state, dirImage] : m_Sprites)
	{
		for (auto& [dir, sprite] : dirImage)
		{
			if (sprite)
				SafeDelete(sprite);
		}
	}

	for (auto& shadow : m_Shadow)
		SafeDelete(shadow.second);

	for (auto Rect : m_vecRect)
		SafeDelete(Rect);
}

void CPlayer::SetSprite(PlayerState _State, PlayerDir _Dir)
{
	if (m_CurImg)
		m_CurImg->visible = false;

	for (auto& shadow : m_Shadow) 
		shadow.second->visible = false;

	auto& stateIter  = m_Sprites[m_PlayerState];
	auto  imgIter = stateIter.find(m_PlayerDir);

	if (imgIter != stateIter.end())
	{
		m_CurImg = imgIter->second;
		m_CurImg->visible = true;

		switch (_State)
		{
		case PlayerState::Idle:
			m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
			break;

		case PlayerState::Walk:
			m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
			break;

		case PlayerState::Dash:
			m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.03f);
			m_CurImg->frame = Int2(0, 0);

			m_DashEndFrame = m_CurImg->maxFrame.x - 1;
			break;
		default: break;
		}
	}

	if (m_Shadow.find(m_PlayerState) != m_Shadow.end())
		m_Shadow[m_PlayerState]->visible = true;

	m_PlayerState = _State;
	m_PlayerDir = _Dir;
}

void CPlayer::Dash()
{
	

}

void CPlayer::Resource_Settings()
{
	// Player Collision
	m_PlayerCol = new ObRect();
	m_PlayerCol->scale = Vec2(100.f, 100.f);
	m_PlayerCol->SetWorldPos(Vec2(200.f, 200.f));
	m_PlayerCol->visible = true;
	m_PlayerCol->isFilled = false;
	m_PlayerCol->pivot = OFFSET_N;
	m_PlayerCol->collider = COLLIDER::RECT;

	m_vecRect.push_back(m_PlayerCol);

	// Idle
	{
		Vec2 idle_Size = Vec2(90.f, 93.f) * 1.6f;
		ObImage* front_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			true);
		front_Idle->SetParentRT(*m_PlayerCol);
		front_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		ObImage* back_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		back_Idle->SetParentRT(*m_PlayerCol);
		back_Idle->SetLocalPos(Vec2(1.5f, 10.f));
		back_Idle->frame = Int2(1, 1);

		ObImage* left_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		left_Idle->SetParentRT(*m_PlayerCol);
		left_Idle->SetLocalPos(Vec2(-15.f, 10.f));
		left_Idle->frame = Int2(2, 1);

		ObImage* right_Idle = RESOURCE->CreateSprite(
			L"Player\\Idle\\Idle.png",
			idle_Size,
			Int2(4, 1),
			false);
		right_Idle->SetParentRT(*m_PlayerCol);
		right_Idle->SetLocalPos(Vec2(-21.f, 10.f));
		right_Idle->frame = Int2(3, 1);

		m_Sprites[PlayerState::Idle][PlayerDir::Front] = front_Idle;
		m_Sprites[PlayerState::Idle][PlayerDir::Back]  = back_Idle;
		m_Sprites[PlayerState::Idle][PlayerDir::Left]  = left_Idle;
		m_Sprites[PlayerState::Idle][PlayerDir::Right] = right_Idle;
	}
	

	// // Walk
	{
		Vec2 walkSize = Vec2(900.f / 10.f, 93.f) * 1.4f;

		ObImage* front_Walk = RESOURCE->CreateSprite(
			L"Player\\Front\\Front_Walk.png", walkSize,
			Int2(10, 1), false);

		front_Walk->SetParentRT(*m_PlayerCol);
		front_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[PlayerState::Walk][PlayerDir::Front] = front_Walk;

		ObImage* back_Walk = RESOURCE->CreateSprite(
			L"Player\\Back\\Back_Walk.png", walkSize,
			Int2(10, 1), false);
		back_Walk->SetParentRT(*m_PlayerCol);
		back_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[PlayerState::Walk][PlayerDir::Back] = back_Walk;;

		ObImage* left_Walk = RESOURCE->CreateSprite(
			L"Player\\Left\\Left_Walk.png", walkSize,
			Int2(10, 1), false);
		left_Walk->SetParentRT(*m_PlayerCol);
		left_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[PlayerState::Walk][PlayerDir::Left] = left_Walk;

		ObImage* right_Walk = RESOURCE->CreateSprite(
			L"Player\\Right\\Right_Walk.png", walkSize,
			Int2(10, 1), false);
		right_Walk->SetParentRT(*m_PlayerCol);
		right_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[PlayerState::Walk][PlayerDir::Right] = right_Walk;
	}

	// Shadow
	{
		ObImage* shadow_Idle = RESOURCE->CreateImage(
			L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f);

		shadow_Idle->SetParentRT(*m_PlayerCol);
		shadow_Idle->SetLocalPos(Vec2(-2.5f,-40.f));
		shadow_Idle->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		shadow_Idle->visible = true;
		m_Shadow.insert({ PlayerState::Idle, shadow_Idle });
		
		ObImage* shadow_Walk = RESOURCE->CreateImage(
			L"Shadow\\MIDDLE_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f);

		shadow_Walk->SetParentRT(*m_PlayerCol);
		shadow_Walk->SetLocalPos(Vec2(-2.5f, -40.f));
		shadow_Walk->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		shadow_Walk->visible = true;
		m_Shadow.insert({ PlayerState::Walk, shadow_Walk });

		//ObImage* shadow_Attack = RESOURCE->CreateImage(
		//	L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.8f, false);
		//
		//shadow_Attack->SetParentRT(*m_PlayerCol);
		//shadow_Attack->SetLocalPos(Vec2(-2.5f, -40.f));
		//shadow_Attack->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		//m_Shadow[PlayerState::Attack] = shadow_Attack;

		// TODO : 공격할 때 그림자 사이즈 조절
		//m_Shadow[PlayerState::Dash] = shadow_Walk;
	}

	// Dash
	{
		Vec2 dash_Size = Vec2(1100.f / 11, 104.f) * 1.5f;

		ObImage* front_Dash = RESOURCE->CreateSprite(
			L"Player\\front\\Front_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		front_Dash->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerState::Dash][PlayerDir::Front] = front_Dash;

		ObImage* back_Dash = RESOURCE->CreateSprite(
			L"Player\\Back\\Back_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		back_Dash->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerState::Dash][PlayerDir::Back] = back_Dash;

		ObImage* left_Dash = RESOURCE->CreateSprite(
			L"Player\\Left\\Left_Dash_long.png",
			dash_Size,
			Int2(11, 1), false);
		left_Dash->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerState::Dash][PlayerDir::Left] = left_Dash;

		ObImage* right_Dash = RESOURCE->CreateSprite(
			L"Player\\Right\\Right_Dash_long.png",
			dash_Size,
			Int2(11, 1),false);
		right_Dash->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerState::Dash][PlayerDir::Right] = right_Dash;

	}

	// Start Image Setting
	m_CurImg = m_Sprites[PlayerState::Idle][PlayerDir::Front];
	SetSprite(m_PlayerState, m_PlayerDir);
}

void CPlayer::Print_ImGui()
{
	if (ImGui::Begin("Player"))
	{
		ImGui::Text("Hp    : %.2f", m_Hp);
		ImGui::Text("Speed : %.2f", m_Speed);
		ImGui::Text("State : %s", CHelper::ToString_PlayerState(m_PlayerState));
		ImGui::Text("Dir   : %s", CHelper::ToString_PlayerDir(m_PlayerDir));
		ImGui::Text("LastDir.x : %f", m_LastDir.x);
		ImGui::Text("LastDir.y : %f", m_LastDir.y);

		ImGui::Text("Dash_Time : %f", m_DashTimer);
		ImGui::InputFloat("Dash_Distance", &m_DashDistance);

		if (m_CurImg)
		{
			Vec2 localPos = m_CurImg->GetLocalPos();

			ImGui::InputFloat2("Local Position", &localPos.x);

			m_CurImg->SetLocalPos(localPos);
		}

	}

	ImGui::End();
}
