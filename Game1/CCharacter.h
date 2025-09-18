#pragma once

#include "CActor.h"

class CStatusComponent;

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter() override;

public:
	void BeginPlay() override abstract  {}
	void Tick()		 override abstract  {}
	void LateUpdate() override abstract {}
	void Render()	 override abstract  {}
	void Release()	 override abstract  {}

public:
	virtual shared_ptr<ObRect> Get_Collision() { return m_Collision; }
	virtual shared_ptr<ObImage> Get_CurImage() { return m_CurrentImage; }

protected:
	virtual void Resource_Settings() abstract;
	virtual void SetSprite(EPlayerState _State, EPlayerDir _Dir) abstract {};

protected:
	vector<CComponent*> m_Components = {};
	CStatusComponent*	m_Status = nullptr;
	shared_ptr<ObRect>	m_Collision = nullptr;
	shared_ptr<ObImage> m_CurrentImage = nullptr;

};

