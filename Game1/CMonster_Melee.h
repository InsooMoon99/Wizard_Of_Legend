#pragma once

#include "CStruct.h"
#include "IMonsterAction.h"

class CMonsterBase;

class CMonster_Melee : public IMonsterAction
{
public:
	CMonster_Melee();
	~CMonster_Melee() override;

public:
	void Settings(const FMeleeParams& params) { m_MeleeParams = params; }

	// Interface
	void OnStart(CMonsterBase* _Owner) override;
	void OnUpdate(CMonsterBase* _Owner, float _delta) override;
	void OnExit(CMonsterBase* _Owner) override;
	bool IsFinished() const override { return m_done; }

private:
	float m_delta = 0.f;

	bool  m_moved = false;
	bool  m_hit = false;
	bool  m_done = false;

	FMeleeParams m_MeleeParams;
};

