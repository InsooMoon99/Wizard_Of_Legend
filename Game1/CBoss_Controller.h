#pragma once

#include "CActor.h"
#include "CDelegate.h"
#include "CStateComponent.h"

class CBossPawn;
class CStatusComponent;
class CStateComponent;
class CUIComponent;

class CBoss_Controller : public CActor
{
public:
	CBoss_Controller();
	virtual ~CBoss_Controller() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void RegisterPawn(CBossPawn* _Pawn);
	void UnRegisterPawn(CBossPawn* _Pawn);

	// Pawn -> Controller 데미지 보고하는 형식으로
	void TakeDamage(float _Damage);
	void ChangeState(ECharacterState _State);
	ECharacterState GetState() { return m_State->Get_State(); }

	shared_ptr<CStatusComponent>	GetStatusComponent() { return m_Status; }
	shared_ptr<CStateComponent>		GetStateComponent() { return m_State; }
	shared_ptr<CUIComponent>		GetUIComponent() { return m_UI; }

public:
	//void SetStart(bool start) { bStart = start; }
	//bool IsStart() { return bStart; }

	Vec2 GetOffSetPos() { return m_OffsetPos; }
	void SetOffsetPos(Vec2 _Pos) { m_OffsetPos = _Pos; }

	Vec2 GetManPos() { return m_ManPos; }
	Vec2 GetGirPos() { return m_GirPos; }
	
	Vec2 GetBallMovePos() { return m_Ball_MovePos; }
	void SetBallMovePos(Vec2 _Pos) { m_Ball_MovePos = _Pos; }

	CDelegate<ECharacterState> OnBossStateChanged;

	CDelegate<> OnBossStart;
	CDelegate<const Vec2&> OnDashStart;

	CDelegate<int> OnThrowTargetOrb;
	CDelegate<const Vec2&> OnJavelinStart;

	CDelegate<const Vec2&> OnStarStart;

	CDelegate<bool> OnPendingKill;

private:
	vector<CBossPawn*> m_Pawns;

	shared_ptr<CStatusComponent>	m_Status;
	shared_ptr<CStateComponent>		m_State;
	shared_ptr<CUIComponent>		m_UI;
		
	Vec2 m_OffsetPos {};
	Vec2 m_ManPos {};
	Vec2 m_GirPos {};
	Vec2 m_Ball_MovePos {};

	int m_ReadyCount = 0;

	float m_AttackTimer = 0.5f;

};

