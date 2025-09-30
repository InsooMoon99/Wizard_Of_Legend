#pragma once

#include "CSkill.h"

class CFire_Dragon;
class CActor;

class CSkill_FireDragon : public CSkill
{
public:
	CSkill_FireDragon();
	virtual ~CSkill_FireDragon();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

	bool CanActivate(const FSkillInfo& _Info) override;
	void Activate(CActor* _Owner) override;

	void Fire();

private:
	vector<shared_ptr<CFire_Dragon>> m_FireDragon;

private:
	CCharacter* m_Character;
	shared_ptr<CStateComponent> m_State;
	Vec2	m_Dir{};
	float	m_Radian;

	int   m_FireCount = 0;     // 남은 발사 수
	float m_ShotDelay = 0.15f; // 샷 간격(초)
	float m_ShotTimer = 0.f;   // 다음 샷까지 남은 시간

	int m_NextWaveMode = 0;
};

