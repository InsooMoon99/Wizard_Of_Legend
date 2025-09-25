#pragma once

#include "CSkill_Actor.h"

class CBoss_TwinGirl;
class CBoss_Controller;

class CStar : public CSkill_Actor
{
public:
	CStar(FSkillInfo& _Info);
	virtual ~CStar();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

private:
	CBoss_Controller* m_BossController;
	CBoss_TwinGirl* m_Boss;

	vector<shared_ptr<ObCircle>> m_Circles;
	vector<shared_ptr<ObImage>>	 m_CircleImg;
	vector<shared_ptr<ObImage>>  m_OrbShadow;

	vector<weak_ptr<CSkill_Actor>> m_Spawned;

	vector<FSkillInfo> m_StartInfo;
	vector<FSkillInfo> m_SecondInfo;
	vector<FSkillInfo> m_ThirdInfo;

	bool bFirst = true;
	bool bSecond = true;
	bool bThird = true;

	float m_Attack_Timer = 0.f;
};

