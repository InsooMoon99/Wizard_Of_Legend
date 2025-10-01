#include "stdafx.h"
#include "CSkill_FireBall.h"
#include "CFire_Ball.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"

CSkill_FireBall::CSkill_FireBall()
{
}

CSkill_FireBall::~CSkill_FireBall()
{
}

void CSkill_FireBall::BeginPlay()
{
    CSkill::BeginPlay();

    m_Id = "FireBall";
    if (auto data = SKILL_DATA->GetById(m_Id))
    {
        m_DataDamage = data->Damage;
        m_DataCoolDown = max(0.f, data->CoolDown - 0.1f);
    }
    else
    {
        m_DataDamage = 10.f;
        m_DataCoolDown = 4.9f; 
    }

}

void CSkill_FireBall::Tick()
{
    CSkill::Tick();

    if (m_Bursting && m_PendingShots > 0)
    {
        m_ShotTimer += DELTA;
        if (m_ShotTimer >= m_ShotInterval)
        {
            m_ShotTimer -= m_ShotInterval;

            FireOnce();
            --m_PendingShots;

            if (m_PendingShots <= 0 && !m_CooldownStarted)
            {
                StartCoolDown();
                m_CooldownStarted = true;
                m_Bursting = false;
            }
        }
    }

    for (auto it = m_FireBall.begin(); it != m_FireBall.end(); )
    {
        auto fireball = *it;
        fireball->Tick();

        if (fireball->IsDestroy()) 
            it = m_FireBall.erase(it);

        else                       
            ++it;
    }
}

void CSkill_FireBall::Render()
{
    CSkill::Render();

    for (auto& fb : m_FireBall)
        if (!fb->IsDestroy()) fb->Render();
}

void CSkill_FireBall::LateUpdate()
{
    CSkill::LateUpdate();
}

void CSkill_FireBall::Release()
{
    CSkill::Release();
}


bool CSkill_FireBall::CanActivate(const FSkillInfo& _Info)
{
    if (m_Bursting) return false;       
    if (!CSkill::CanActivate(_Info)) return false;
    return true;
}

void CSkill_FireBall::Activate(CActor* _Owner)
{
    if (!CanActivate(m_Skill_Info)) return;

    CSkill::Activate(_Owner);

    auto character = dynamic_cast<CCharacter*>(_Owner);
    auto state = character ? CHelper::GetComponent<CStateComponent>(character) : nullptr;
    auto status = character ? CHelper::GetComponent<CStatusComponent>(character) : nullptr;
    if (!character || !state || !status) return;

    const bool enhance = (status->GetMp() >= status->GetMaxMp());

    m_Skill_Info.Instigator = _Owner;

    if (enhance)
    {
        m_Bursting = true;
        m_PendingShots = 3;
        m_ShotTimer = 0.f;
        m_CooldownStarted = false;

        FireOnce();
        --m_PendingShots;

        TIMER->HitStop(0.05f, 0.36f, 0.06f);
        PARTICLE->Spawn_Particle(L"Enhance", character->Get_WorldPos());
        SOUND->StopPlay("EnhanceCharge", 0.4f);
    }
    else
    {
        FireOnce();
        StartCoolDown();
        m_CooldownStarted = true;
    }
}

void CSkill_FireBall::FireOnce()
{
    auto owner = m_Skill_Info.Instigator;
    auto character = dynamic_cast<CCharacter*>(owner);
    if (!character) return;

    auto state = CHelper::GetComponent<CStateComponent>(character);
    if (!state) return;

    auto status = CHelper::GetComponent<CStatusComponent>(character);
    if (!status) return;

    state->Change_State(state->Set_AttackType());

    Vec2  dir = CHelper::DirToVec(state->Get_Dir());
    float rad = Utility::DirToRadian(dir);

    FSkillInfo info = m_Skill_Info;     
    info.SpawnPos = owner->Get_WorldPos();
    info.Aim_Dir = dir;
    info.Angle = rad;
    info.Damage = m_DataDamage + (status->GetAttack() * m_AttackScale);
    info.Amount = 1000.f;

    auto fireBall = make_shared<CFire_Ball>(info);
    fireBall->BeginPlay();
    m_FireBall.push_back(fireBall);
}
