#pragma once

#include "CCharacter.h"
#include "CEnums.h"
#include "CStruct.h"

class CPlayer : public CCharacter
{
public:
	CPlayer();
	virtual ~CPlayer() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

public:
	void SetState(PlayerState _State, PlayerDir _Dir);

protected:
	void Resource_Settings() override;

private:
	void Print_ImGui();

private:
	PlayerState		 m_PlayerState = PlayerState::Idle;
	PlayerDir		 m_PlayerDir;

	Vec2 moveDir = { 0, 0 };

	// Rect
	vector<ObRect*>  m_vecRect;
	ObRect*			 m_PlayerCol;

	// Image
	unordered_map<PlayerDir, ObImage*> m_Sprites;
	ObImage* m_CurImg = nullptr;

};

