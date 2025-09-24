#pragma once

#include "CSkill.h"

class CTarget_Orb;
class CActor;
class CBossPawn;
class CBoss_Controller;

class CSkill_Man_Target : public CSkill
{
public:
    CSkill_Man_Target();
    virtual ~CSkill_Man_Target();

public:
    void BeginPlay() override;
    void Tick() override;
    void LateUpdate() override;
    void Render() override;
    void Release() override;

    bool CanActivate(const FSkillInfo& _Info) override;
    void Activate(CActor* _Owner) override;

private:
    void BindDelegateOnce(CBoss_Controller* ctrl);
    void CleanupDeadOrbs();

private:
    vector<std::shared_ptr<CTarget_Orb>> m_Orbs;

    bool m_DelegateBound = false;

    CActor* m_TargetCached = nullptr;

    static constexpr int   kNumOrbs = 5;    
    static constexpr float kBaseDmg = 10.f;
    static constexpr float kKnockAmt = 1000.f;
};
