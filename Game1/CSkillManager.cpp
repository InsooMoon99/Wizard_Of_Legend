#include "stdafx.h"
#include "CSkillManager.h"

#include "CAttackActor.h"
#include "CBat_Dash_Shock.h"
#include "CSkill_Actor.h"
#include "CStar_Collision.h"

CSkillManager* CSkillManager::Instance = nullptr;

void CSkillManager::Init()
{
	
}

void CSkillManager::Update()
{
    for (auto it = m_ActiveSkills.begin(); it != m_ActiveSkills.end();)
    {
        auto skill = *it;
        skill->Tick();

        if (skill->IsDestroy())
        {
            it = m_ActiveSkills.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void CSkillManager::Render()
{
    for (auto& skill : m_ActiveSkills)
    {
        skill->Render();
    }
}

void CSkillManager::Release()
{
    m_ActiveSkills.clear();
}

shared_ptr<CSkill_Actor> CSkillManager::Spawn_SkillActor(ESkillActor _Skill, FSkillInfo& _Info, const Vec2& size)
{
    shared_ptr<CSkill_Actor> newSkill = nullptr;

    switch (_Skill)
    {
    case ESkillActor::Dash_Shock:
        newSkill = make_shared<CBat_Dash_Shock>(_Info);
        break;

    case  ESkillActor::AttackCollision:
        newSkill = make_shared<CAttackActor>(_Info, size);

    }

    if (newSkill)
    {
        newSkill->BeginPlay();
        m_ActiveSkills.push_back(newSkill);
    }

    return newSkill;
}

shared_ptr<CSkill_Actor> CSkillManager::Spawn_SkillActor(ESkillActor _Skill, FSkillInfo& _Info, EStarType _Star)
{
    shared_ptr<CSkill_Actor> newSkill = nullptr;

    switch (_Skill)
    {
    case ESkillActor::Star_Collision:
        newSkill = make_shared<CStar_Collision>(_Info, _Star);
        break;

    }

    if (newSkill)
    {
        newSkill->BeginPlay();
        m_ActiveSkills.push_back(newSkill);
    }

    return newSkill;
}
