#pragma once

#include "CActor.h"

class CStatusComponent;

class CCharacter : public CActor
{
public:
	CCharacter();
	virtual ~CCharacter() override;

public:
	void BeginPlay() override abstract {}
	void Tick()		 override abstract {}
	void LateUpdate() override abstract {}
	void Render()	 override abstract {}
	void Release()	 override abstract {}

protected:
	virtual void Resource_Settings() abstract;

public:
	virtual void AddComponent(CComponent* _Component);
	virtual void RemoveComponent(CComponent* _Component);

protected:
	vector<CComponent*> m_Components;

	CStatusComponent* m_Status;

};

