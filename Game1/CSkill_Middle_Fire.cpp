#include "stdafx.h"
#include "CSkill_Middle_Fire.h"
#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"

CSkill_Middle_Fire::CSkill_Middle_Fire() {}
CSkill_Middle_Fire::~CSkill_Middle_Fire() {}

void CSkill_Middle_Fire::BeginPlay()
{
    CSkill::BeginPlay();
}

void CSkill_Middle_Fire::Tick()
{
    CSkill::Tick();

    // === 버스트 타이밍 처리 ===
    if (m_Casting && m_WavesLeft > 0)
    {
        m_WaveTimer += DELTA;
        if (m_WaveTimer >= m_WaveInterval)
        {
            // 여러 프레임 스킵 대비, 필요한 만큼 뽑기
            while (m_WaveTimer >= m_WaveInterval && m_WavesLeft > 0)
            {
                m_WaveTimer -= m_WaveInterval;
                SpawnWave(m_OwnerCache);
                --m_WavesLeft;
            }
        }

        // 버스트 종료
        if (m_WavesLeft <= 0)
        {
            m_Casting = false;
            m_WaveTimer = 0.f;
        }
    }

    // === 생성된 오브 업데이트 ===
    for (auto iter = m_Ball.begin(); iter != m_Ball.end(); )
    {
        auto ball = *iter;
        ball->Tick();

        if (ball->IsDestroy())
            iter = m_Ball.erase(iter);
        else
            ++iter;
    }
}

void CSkill_Middle_Fire::LateUpdate()
{
    CSkill::LateUpdate();
}

void CSkill_Middle_Fire::Render()
{
    CSkill::Render();

    for (auto& ball : m_Ball)
        if (!ball->IsDestroy())
            ball->Render();
}

void CSkill_Middle_Fire::Release()
{
    CSkill::Release();
    m_Casting = false;
    m_WavesLeft = 0;
    m_WaveTimer = 0.f;
    m_Ball.clear();
}

bool CSkill_Middle_Fire::CanActivate(const FSkillInfo& _Info)
{
    if (!CSkill::CanActivate(_Info))
        return false;

    // 보스는 항상 true로 둬도 됨. 필요시 조건 추가
    return true;
}

void CSkill_Middle_Fire::Activate(CActor* _Owner)
{
    CSkill::Activate(_Owner);

    CCharacter* character = dynamic_cast<CCharacter*>(_Owner);
    if (!character) return;

    auto state = CHelper::GetComponent<CStateComponent>(character);
    auto status = CHelper::GetComponent<CStatusComponent>(character);
    if (!state || !status) return;

    m_OwnerCache = _Owner;
    m_Casting = true;
    m_WavesLeft = 3;       
    m_WaveTimer = 0.f;

    SpawnWave(_Owner);
    --m_WavesLeft;
}

void CSkill_Middle_Fire::SpawnWave(CActor* owner)
{
    if (!owner) return;

    Vec2 center = owner->Get_WorldPos();

    const float step = (2.f * PI) / (float)m_NumOrbs;

    for (int i = 0; i < m_NumOrbs; ++i)
    {
        float ang = i * step;
        Vec2  offset = Vec2(cosf(ang), sinf(ang)) * m_Radius;

        Vec2 dirOut = offset;
        if (dirOut.LengthSquared() > 1e-6f) dirOut.Normalize();

        m_Skill_Info.Instigator = owner;
        m_Skill_Info.Target = nullptr;              
        m_Skill_Info.SpawnPos = center + offset;    
        m_Skill_Info.Aim_Dir = dirOut;              
        m_Skill_Info.Angle = ang;
        m_Skill_Info.Amount = m_Speed;              
        m_Skill_Info.Damage = 20.f;

        auto orb = make_shared<CMiddle_Fire>(m_Skill_Info);
        orb->BeginPlay();

        m_Ball.emplace_back(orb);
    }
}
