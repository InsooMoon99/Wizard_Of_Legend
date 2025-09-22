#include "stdafx.h"
#include "CWeaponComponent.h"

#include "CSkill_Factory.h"
#include "CSkill_FireBall.h"
#include "CSkill_FireDragon.h"

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

    for (auto& kv : m_Skills)
        kv.second->Tick();
}

void CWeaponComponent::Render()
{
    CComponent::Render();

    for (auto& kv : m_Skills)
        kv.second->Render();
}

void CWeaponComponent::Release()
{
    CComponent::Release();
    m_Skills.clear();
}

void CWeaponComponent::Equip_Skill(ESkillType _SkillType)
{
    auto it = m_Skills.find(_SkillType);
    if (it == m_Skills.end())
    {
        std::shared_ptr<CSkill> newSkill = CSkill_Factory::CreateSkill(_SkillType);
        if (newSkill)
        {
            m_Skills[_SkillType] = newSkill;
            newSkill->BeginPlay();
        }
    }
}

void CWeaponComponent::Activate_Skill(ESkillType _SkillType)
{
    auto it = m_Skills.find(_SkillType);
    if (it != m_Skills.end())
    {
        // 연속발사 스킬일 경우, 발사 시작을 명시
        if (_SkillType == ESkillType::Fire_Dragon)
        {
            if (auto fireDragon = std::dynamic_pointer_cast<CSkill_FireDragon>(it->second))
                fireDragon->SetFiring(true);
        }

        it->second->Activate(GetOwner());
    }
}

// ★ 추가: 연속 발사 중인지(= 애니메이션을 다시 재생해야 하는지) 확인
bool CWeaponComponent::IsFiringSkill()
{
    // 지금은 Fire_Dragon만 체크해도 충분
    auto it = m_Skills.find(ESkillType::Fire_Dragon);
    if (it != m_Skills.end())
    {
        if (auto fireDragon = dynamic_pointer_cast<CSkill_FireDragon>(it->second))
            return fireDragon->GetFiring();
    }
    return false;
}
