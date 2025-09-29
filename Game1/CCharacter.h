#pragma once

#include "CActor.h"
#include "IColorChanger.h"

class CStatusComponent;
class CStateComponent;

class CCharacter : public CActor, public IColorChanger
{
public:
	CCharacter();
	virtual ~CCharacter() override;

public:
	void BeginPlay() override;
	void Tick()		 override;
	void LateUpdate() override;
	void Render()	 override;
	void Release()	 override;

public:
	void Hit_Color()  override abstract {};
	void Hit_Effect() override abstract {};

	bool IsKnockbackPrev() { return m_bKnockback; }

	void SetTeamID(int _ID) { m_TeamID = _ID; }
	int  GetTeamID() { return m_TeamID; }

	virtual void TakeDamage(FHitDamage _Hit);

protected:
	shared_ptr<CStatusComponent> m_Status;
	shared_ptr<CStateComponent> m_State;
	
protected:
	virtual void Resource_Settings() abstract {};
	virtual void SetSprite(ECharacterState _State, EPlayerDir _Dir) {};
	virtual void Play_Sprite(ECharacterState _State) {}

protected:
	unordered_map<ECharacterState, unordered_map<EPlayerDir, shared_ptr<ObImage>>> m_Sprites;

	// Components
	vector<CComponent*> m_Components = {};

	ECharacterState m_prevState;
	EPlayerDir   m_prevDir;

	float m_HitTimer = 0.f;
	bool  m_bKnockback = true;
	int	  m_TeamID = 1;

public:
	bool IsKnockback() const { return m_Knock.active; }
	void UpdateKnockbackTimer();

	void SetDead();

protected:
	FKnockbackState m_Knock;
};

