#pragma once

#include "CSkill_Actor.h"

class CGaia_Armor : public CSkill_Actor
{
public:
	CGaia_Armor(SkillInfo& _Info);
	virtual ~CGaia_Armor();

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
	shared_ptr<ObImage>  m_Armor_Img[8];
	shared_ptr<ObCircle> m_Armor[8];

	float m_Angle[8] = { 0.f };
	float m_Radius = 100.f;

};

