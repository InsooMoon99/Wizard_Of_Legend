#include "stdafx.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
	testRect = new ObRect();
	testRect->scale = Vec2(100.f, 100.f);
	testRect->SetWorldPos(Vec2(200.f, 200.f));
	testRect->visible = true;
	testRect->isFilled = false;
	testRect->pivot = OFFSET_N;
	testRect->collider = COLLIDER::RECT;

}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::BeginPlay()
{
	CCharacter::BeginPlay();

	
}

void CPlayer::Tick()
{
	CCharacter::Tick();

	testRect->Update();
}

void CPlayer::Render()
{
	CCharacter::Render();

	testRect->Render();
}

void CPlayer::Release()
{
	CCharacter::Release();

	SafeDelete(testRect)
}
