#include "stdafx.h"
#include "CPlayer.h"

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

	moveDir = Vec2(0.f, 0.f);

	if (INPUT->KeyPress('S'))
	{
		moveDir.y = -1.f;
		m_PlayerState = PlayerState::Walk;
		m_PlayerDir = PlayerDir::Front;
	}

	if (INPUT->KeyPress('W'))
	{
		moveDir.y = 1.f;
		m_PlayerState = PlayerState::Walk;
		m_PlayerDir = PlayerDir::Back;
	}

	if (INPUT->KeyPress('A'))
	{
		moveDir.x = -1.0f;
		m_PlayerState = PlayerState::Walk;
		m_PlayerDir = PlayerDir::Left;
	}

	if (INPUT->KeyPress('D'))
	{
		moveDir.x = 1.f;
		m_PlayerState = PlayerState::Walk;
		m_PlayerDir = PlayerDir::Right;
	}

	// Stop
	if (moveDir.x == 0 && moveDir.y == 0) 
		m_PlayerState = PlayerState::Idle;

	SetState(m_PlayerState, m_PlayerDir);

	m_PlayerCol->MoveWorldPos(moveDir * 400.f * DELTA);

	for (auto& Rect : m_vecRect)
		Rect->Update();

	for (auto& Image : m_Sprites)
		Image.second->Update();

}

void CPlayer::Render()
{
	for (auto& Rect : m_vecRect)
		Rect->Render();

	for (auto& Image : m_Sprites)
		Image.second->Render();
}

void CPlayer::Release()
{
	for (auto Rect : m_vecRect)
		SafeDelete(Rect)

	for (auto Image : m_Sprites)
		SafeDelete(Image.second)
}

void CPlayer::SetState(PlayerState _State, PlayerDir _Dir)
{
	if (m_CurImg) 
		m_CurImg->visible = false;

	auto Image = m_Sprites.find(_Dir);
	if (Image != m_Sprites.end())
	{
		m_CurImg = Image->second;
		m_CurImg->visible = true;
	}
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

	m_Sprites[PlayerDir::Front] = new ObImage(L"Player\\FRONT_00-09_spritesheet.png");
	m_Sprites[PlayerDir::Back]  = new ObImage(L"Player\\BACK_COMPLETE.bmp");
	m_Sprites[PlayerDir::Left]  = new ObImage(L"Player\\LEFT_COMPLETE.bmp");
	m_Sprites[PlayerDir::Right] = new ObImage(L"Player\\RIGHT_COMPLETE.bmp");

	// Player Front Image
	{
		m_Sprites[PlayerDir::Front]->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerDir::Front]->scale = Vec2(802.f / 10.f, 83.f);
		m_Sprites[PlayerDir::Front]->maxFrame = Int2(10, 1);
		m_Sprites[PlayerDir::Front]->frame = Int2(0, 0);
		m_Sprites[PlayerDir::Front]->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	}
	
	// Player Back Image
	{
		m_Sprites[PlayerDir::Back]->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerDir::Back]->scale = Vec2(960.f / 11.f, 548.f / 6.f);
		m_Sprites[PlayerDir::Back]->maxFrame = Int2(11, 6);
		m_Sprites[PlayerDir::Back]->frame = Int2(0,0);
		m_Sprites[PlayerDir::Back]->visible = false;
	}

	// Player Left Image
	{
		m_Sprites[PlayerDir::Left]->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerDir::Left]->scale = Vec2(960.f / 11.f, 546.f / 6.f);
		m_Sprites[PlayerDir::Left]->maxFrame = Int2(11, 6);
		m_Sprites[PlayerDir::Left]->frame = Int2(0, 0);
		m_Sprites[PlayerDir::Left]->visible = false;
	}

	// Player Right Image
	{
		m_Sprites[PlayerDir::Right]->scale = Vec2(960.f / 11.f, 546.f / 6.f);
		m_Sprites[PlayerDir::Right]->SetParentRT(*m_PlayerCol);
		m_Sprites[PlayerDir::Right]->maxFrame = Int2(11, 6);
		m_Sprites[PlayerDir::Right]->frame = Int2(0, 0);
		m_Sprites[PlayerDir::Right]->visible = false;
	}

	// Start Image Setting
	m_CurImg = m_Sprites[PlayerDir::Front];

	
}

void CPlayer::Print_ImGui()
{
	if (ImGui::Begin("Player"))
	{
		ImGui::Text("Hp    : %.2f", m_Hp);
		ImGui::Text("Speed : %.2f", m_Speed);
		ImGui::Text("State : %s", CHelper::ToString_PlayerState(m_PlayerState));
		ImGui::Text("Dir   : %s", CHelper::ToString_PlayerDir(m_PlayerDir));
	}
	
	ImGui::End();
}
