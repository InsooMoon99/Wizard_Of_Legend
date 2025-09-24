#pragma once

#include "CSkill.h"

class CSpawn_Rect;

class CSkill_Girl_SpawnRect : public CSkill
{
public:
	CSkill_Girl_SpawnRect();
	virtual ~CSkill_Girl_SpawnRect();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	bool CanActivate(const FSkillInfo& _Info) override;
	void Activate(CActor* _Owner) override;

private:
	vector<shared_ptr<CSpawn_Rect>> m_Rect{};
};

