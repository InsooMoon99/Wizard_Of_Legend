#include "stdafx.h"
#include "CSkill_Man_Target.h"

#include "CTarget_Orb.h"
#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CStateComponent.h"
#include "CActor.h"

CSkill_Man_Target::CSkill_Man_Target() {}
CSkill_Man_Target::~CSkill_Man_Target()
{
	Release();
}

void CSkill_Man_Target::BeginPlay()
{
    CSkill::BeginPlay();
}

void CSkill_Man_Target::Tick()
{
    CSkill::Tick();

    for (auto& orb : m_Orbs)
    {
        if (orb)
            orb->Tick();
    }
        
    CleanupDeadOrbs();
}

void CSkill_Man_Target::LateUpdate()
{
    CSkill::LateUpdate();
}

void CSkill_Man_Target::Render()
{
    CSkill::Render();

    for (auto& orb : m_Orbs)
        if (orb) 
            orb->Render();
}

void CSkill_Man_Target::Release()
{
    m_DelegateBound = false;
    m_TargetCached = nullptr;

    m_Orbs.clear();

    CSkill::Release();
}

bool CSkill_Man_Target::CanActivate(const FSkillInfo& _Info)
{
    if (_Info.Target == nullptr)
        return false;

    return CSkill::CanActivate(_Info);
}

void CSkill_Man_Target::Activate(CActor* _Owner)
{
    CSkill::Activate(_Owner);
    if (_Owner == nullptr)
        return;

    CBossPawn* boss = dynamic_cast<CBossPawn*>(_Owner);
    if (!boss) return;

    auto state = boss->GetControlState();
    if (!state) return;

    m_TargetCached = boss->GetPlayer();
    if (!m_TargetCached) return;

    Vec2 dir = CHelper::DirToVec(state->Get_Dir());
    if (dir.LengthSquared() < 1e-6f)
        dir = Vec2(1.f, 0.f);

    m_Orbs.clear();
    m_Orbs.reserve(kNumOrbs);

    const float angleStep = (2.0f * PI) / static_cast<float>(kNumOrbs);

    for (int i = 0; i < kNumOrbs; ++i)
    {
        m_Skill_Info.Instigator = _Owner;
        m_Skill_Info.SpawnPos = _Owner->Get_WorldPos();
        m_Skill_Info.Target = m_TargetCached;      
        m_Skill_Info.Aim_Dir = dir;
        m_Skill_Info.Angle = i * angleStep;      
        m_Skill_Info.Damage = kBaseDmg;
        m_Skill_Info.Amount = kKnockAmt;

        auto orb = make_shared<CTarget_Orb>(m_Skill_Info);
        orb->BeginPlay();                             
        m_Orbs.push_back(orb);
    }

    BindDelegateOnce(boss->GetController());
}

void CSkill_Man_Target::BindDelegateOnce(CBoss_Controller* ctrl)
{
    if (!ctrl) return;
    if (m_DelegateBound) return;

    ctrl->OnThrowTargetOrb.Add([this](int idx)
        {
            if (idx < 0 || idx >= static_cast<int>(m_Orbs.size()))
                return;

            auto& orb = m_Orbs[idx];
            if (!orb) return; 

            if (m_TargetCached)
                orb->ThrowTowards(m_TargetCached->Get_WorldPos());
        });

    m_DelegateBound = true;
}

void CSkill_Man_Target::CleanupDeadOrbs()
{
    for (auto& o : m_Orbs)
    {
        if (o && o->IsDestroy())
            o.reset(); // nullptr
    }
}
