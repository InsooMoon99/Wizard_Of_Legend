#pragma once

#include "CMiddle_SpawnBall.h"
#include "CSkill.h"

class CSkill_Middle_SpawnBall : public CSkill
{
public:
	CSkill_Middle_SpawnBall();
	virtual ~CSkill_Middle_SpawnBall();

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
	vector<shared_ptr<CMiddle_SpawnBall>> m_Ball{};
};

