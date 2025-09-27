#include "stdafx.h"
#include "CSkill_FireDragon.h"
#include "CFire_Dragon.h"
#include "CCharacter.h"
#include "CStateComponent.h"

CSkill_FireDragon::CSkill_FireDragon()
{
}

CSkill_FireDragon::~CSkill_FireDragon()
{
    Release();
}

void CSkill_FireDragon::BeginPlay()
{
    CSkill::BeginPlay();

    m_Id = "FireDragon";

    if (auto data = SKILL_DATA->GetById(m_Id))
        m_DataDamage = data->Damage;
    else
        m_DataDamage = 10.f;

    //SetIcon(L"UI\\ShootFireArc.png", Vec2(16.f, 16.f) * 2.5f);
}

void CSkill_FireDragon::Tick()
{
    CSkill::Tick();

    if (bFiring)
    {
        m_ShotTimer -= DELTA;

        if (m_ShotTimer <= 0.f && m_FireCount > 0)
        {
            if (m_State)
                m_State->Change_State(m_State->Set_AttackType());

            Fire();

            m_FireCount--;
            m_ShotTimer += m_ShotDelay;

            if (m_FireCount <= 0)
                bFiring = false;
        }
    }

    for (auto it = m_FireDragon.begin(); it != m_FireDragon.end(); )
    {
        auto& proj = *it;
        proj->Tick();

        if (proj->IsDestroy())
            it = m_FireDragon.erase(it);
        else
            ++it;
    }
}

void CSkill_FireDragon::LateUpdate()
{
    CSkill::LateUpdate();

}

void CSkill_FireDragon::Render()
{
    CSkill::Render();

    for (auto& fireDragon : m_FireDragon)
    {
        if (!fireDragon->IsDestroy())
            fireDragon->Render();
    }
}

void CSkill_FireDragon::Release()
{
    CSkill::Release();
}

bool CSkill_FireDragon::CanActivate(const FSkillInfo& _Info)
{
    if (!CSkill::CanActivate(_Info))
        return false;

    // TODO: 쿨타임/마나 등 조건 추가
    return true;
}

void CSkill_FireDragon::Activate(CActor* _Owner)
{
    CSkill::Activate(_Owner);

    m_Character = dynamic_cast<CCharacter*>(_Owner);
    if (!m_Character) return;

    m_State = CHelper::GetComponent<CStateComponent>(m_Character);
    if (!m_State) return;

    m_ShotDelay = 0.15f;   
    m_ShotTimer = 0.f;     
    m_FireCount = 8;       
    bFiring = true;

    m_State->Change_State(m_State->Set_AttackType());
}

void CSkill_FireDragon::Fire()
{
    m_Dir = CHelper::DirToVec(m_State->Get_Dir());
    float rad = Utility::DirToRadian(m_Dir);

    m_Skill_Info.Instigator = m_Character;
    m_Skill_Info.SpawnPos = m_Character->Get_WorldPos();
    m_Skill_Info.Aim_Dir = m_Dir;
    m_Skill_Info.Angle = rad;
    m_Skill_Info.Damage = m_DataDamage;
    m_Skill_Info.Amount = 1000.f;

    auto fireDragon = std::make_shared<CFire_Dragon>(m_Skill_Info);
    fireDragon->BeginPlay();

    m_FireDragon.push_back(fireDragon);
}
