#pragma once

#include "CActor.h"

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

	// Pawn -> Controller 데미지 보고
	void TakeDamage(float _Damage);

	shared_ptr<CStatusComponent> GetStatus() { return m_Status; }
	shared_ptr<CStateComponent> GetState() { return m_State; }
	shared_ptr<CUIComponent> GetUI() { return m_UI; }


private:
	vector<CBossPawn*> m_Pawns;

	shared_ptr<CStatusComponent> m_Status;
	shared_ptr<CStateComponent> m_State;
	shared_ptr<CUIComponent> m_UI;

	int m_Phase = 1;

};

