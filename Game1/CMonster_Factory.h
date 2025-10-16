#pragma once

class IMonsterAction;

class CMonster_Factory
{
public:
	CMonster_Factory()  = default;
	~CMonster_Factory() = default;

public:
	static unique_ptr<IMonsterAction> CreateMelee(CMonsterBase* _Owner, const FMeleeParams& params);

};

