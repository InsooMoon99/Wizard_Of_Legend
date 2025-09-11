#include "stdafx.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::BeginPlay()
{
	CCharacter::BeginPlay();

}

void CPlayer::Tick()
{
	CCharacter::Tick();

}

void CPlayer::Render()
{
	CCharacter::Render();

	int a = 32;
}

void CPlayer::Release()
{
	CCharacter::Release();
}
