#pragma once

class CCharacter;

class IMonsterAction
{
public:
	IMonsterAction() {};
	virtual ~IMonsterAction() {};

public:
	virtual void OnStart(CMonsterBase* _Owner) abstract;
	virtual void OnUpdate(CMonsterBase* _Owner, float _delta) abstract;
	virtual void OnExit(CMonsterBase* _Owner) abstract;
	virtual bool IsFinished() const abstract;

};

