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

protected:
	CStatusComponent* m_Status = nullptr;
	CStateComponent* m_State = nullptr;
	
protected:
	virtual void Resource_Settings() abstract {};
	virtual void SetSprite(EPlayerState _State, EPlayerDir _Dir) abstract {};
	virtual void Play_Sprite(EPlayerState _State) abstract {}

protected:
	unordered_map<EPlayerState, unordered_map<EPlayerDir, shared_ptr<ObImage>>> m_Sprites;

	// Components
	vector<CComponent*> m_Components = {};

	EPlayerState m_prevState;
	EPlayerDir   m_prevDir;

	float m_HitTimer = 0.f;
};

