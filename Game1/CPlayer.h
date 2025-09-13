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
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void SetSprite(PlayerState _State, PlayerDir _Dir);
	void Dash();

	float m_DashTimer = 0.4f;
	int	  m_DashEndFrame = 11;

protected:
	void Resource_Settings() override;

private:
	void Print_ImGui();

private:
	PlayerState		 m_PlayerState = PlayerState::Idle;
	PlayerDir		 m_PlayerDir   = PlayerDir::Front;

	Vec2 m_MoveDir = { 0, 0 };
	Vec2 m_LastDir = { 0, 0 };

	float m_DashDistance = 1200.f;

	// Collision
	vector<ObRect*>   m_vecRect;
	vector<ObCircle*> m_vecCircle;
	ObRect*			  m_PlayerCol;

	// Image
	unordered_map<PlayerState, unordered_map<PlayerDir, ObImage*>> m_Sprites;
	unordered_map<PlayerState, ObImage*> m_Shadow;

	ObImage* m_CurImg = nullptr;

	// Render Sequence 
	vector<RenderEntry> m_RenderQueue;

};

