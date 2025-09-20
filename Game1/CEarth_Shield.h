#pragma once

#include "CSkill_Actor.h"

class CEarth_Shield : public CSkill_Actor
{
public:
	CEarth_Shield(SkillInfo& _Info);
	virtual ~CEarth_Shield();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override;
	void OnComponentEndOverlap(GameObject* collider, GameObject* other) override;

private:
	vector<shared_ptr<ObCircle>> m_Armor;
	vector<shared_ptr<ObImage>> m_Armor_Img;

	float m_Angle[EARTH_SHIELD] = { 0.f };
	float m_Radius = 100.f;

	float m_SpawnTime = 4.f;
};

