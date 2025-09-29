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

    for (auto& skill : m_Skills)
        skill.second->Tick();
}

void CWeaponComponent::Render()
{
    CComponent::Render();

    for (auto& skill : m_Skills)
        skill.second->Render();
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
    vector<FSkill_UiInfo> out(SKILL_PANEL);
    //out.reserve(SKILL_PANEL);

    for (int i = 0; i < SKILL_PANEL; ++i)
        out[i].Slot = (ESkillSlot)i;

    for (auto& [type, skill] : m_Skills)
    {
        if (!skill) continue;
        int slot = (int)skill->GetSlot();
        if (slot < 0 || slot >= (int)out.size()) 
            continue;

        FSkill_UiInfo   ui {};

        ui.Slot = skill->GetSlot();
        ui.Type = type;

        ui.Cooldown = skill->GetCoolDown();
        ui.CooldownRemain = skill->GetCoolRemain();
        ui.IsCoolTime = skill->IsCoolTime();

        ui.Id = CHelper::SkillIdFromType(type);

        auto data = SKILL_DATA->GetById(ui.Id);
        if (data)
        {
            ui.Name = data->Name;
            ui.Desc = data->Desc;
            ui.Icon = SKILL_DATA->GetIconById(ui.Id);
        }

        out[slot] = std::move(ui);
    }

    return out;
}
