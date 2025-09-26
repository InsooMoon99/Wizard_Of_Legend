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

void CWeaponComponent::Equip_Skill(ESkillType _SkillType, ESkillSlot _Slot)
{
    auto it = m_Skills.find(_SkillType);
    if (it == m_Skills.end())
    {
        shared_ptr<CSkill> newSkill = CSkill_Factory::CreateSkill(_SkillType);
        if (newSkill)
        {
            newSkill->BeginPlay();
            newSkill->SetSlot(_Slot);
            m_Skills[_SkillType] = newSkill;
        }
    }
    else
    {
        it->second->SetSlot(_Slot);
    }
}

void CWeaponComponent::Activate_Skill(ESkillType _SkillType)
{
    auto it = m_Skills.find(_SkillType);
    if (it != m_Skills.end())
    {
        if (_SkillType == ESkillType::Fire_Dragon)
        {
            if (auto fireDragon = std::dynamic_pointer_cast<CSkill_FireDragon>(it->second))
                fireDragon->SetFiring(true);
        }

        it->second->Activate(GetOwner());
    }
}

bool CWeaponComponent::IsFiringSkill()
{
    auto it = m_Skills.find(ESkillType::Fire_Dragon);
    if (it != m_Skills.end())
    {
        if (auto fireDragon = dynamic_pointer_cast<CSkill_FireDragon>(it->second))
            return fireDragon->GetFiring();
    }
    return false;
}

vector<FSkill_UiInfo> CWeaponComponent::GetSkill_UiInfo()
{
    vector<FSkill_UiInfo> skill_Info;

    for (auto& [type, skill] : m_Skills)
    {
        if (!skill) continue;

        FSkill_UiInfo   info;

        info.Type       = type;
        info.Slot       = skill->GetSlot();
        info.Icon       = skill->GetIcon();
        info.Cooldown   = skill->Get_CoolDown();

        skill_Info.push_back(info);
    }

    sort(skill_Info.begin(), skill_Info.end(), [](FSkill_UiInfo src, FSkill_UiInfo& dst)
        {
            return (int)src.Slot < (int)dst.Slot;
        });


    return skill_Info;
}
