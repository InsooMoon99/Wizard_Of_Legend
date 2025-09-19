#include "stdafx.h"
#include "CWeaponComponent.h"

#include "CSkill_Factory.h"
#include "CSkill_FireBall.h"

CWeaponComponent::CWeaponComponent()
{

}

CWeaponComponent::~CWeaponComponent()
{
}

void CWeaponComponent::BeginPlay()
{
	CComponent::BeginPlay();


}

void CWeaponComponent::TickComponent()
{
	CComponent::TickComponent();

	for (auto& skill : m_Skills)
	{
		skill.second->Tick();
	}
}

void CWeaponComponent::Render()
{
	CComponent::Render();

	for (auto& skill : m_Skills)
	{
		skill.second->Render();
	}
}

void CWeaponComponent::Release()
{
	CComponent::Release();

	m_Skills.clear();
}

void CWeaponComponent::Equip_Skill(ESkillType _SkillType)
{
	auto find_iter = m_Skills.find(_SkillType);

	if (find_iter == m_Skills.end())
	{
		shared_ptr<CSkill> newSkill = CSkill_Factory::CreateSkill(_SkillType);
		if (newSkill)
		{
			m_Skills[_SkillType] = newSkill;
			newSkill->BeginPlay();
		}
	}
}

void CWeaponComponent::Activate_Skill(ESkillType _SkillType)
{
	auto find_iter = m_Skills.find(_SkillType);

	if (find_iter != m_Skills.end())
	{
		m_Skills[_SkillType]->Activate(GetOwner());
	}
}
