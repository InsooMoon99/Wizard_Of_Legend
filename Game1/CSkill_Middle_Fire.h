#pragma once
#include "CMiddle_Fire.h"
#include "CSkill.h"

class CSkill_Middle_Fire : public CSkill
{
public:
    CSkill_Middle_Fire();
    virtual ~CSkill_Middle_Fire();

public:
    void BeginPlay() override;
    void Tick() override;
    void LateUpdate() override;
    void Render() override;
    void Release() override;

public:
    bool CanActivate(const FSkillInfo& _Info) override;
    void Activate(CActor* _Owner) override;

private:
    // 기존
    vector<shared_ptr<CMiddle_Fire>> m_Ball;

    // === 버스트 컨트롤 ===
    bool  m_Casting = false;    // 버스트 진행 중 여부
    int   m_WavesLeft = 0;        // 남은 웨이브 수
    float m_WaveTimer = 0.f;      // 웨이브 간 타이머
    float m_WaveInterval = 0.4f;     // 웨이브 간격(초)
    CActor* m_OwnerCache = nullptr;  // Activate 때 받은 오너

    // === 패턴 파라미터 ===
    int   m_NumOrbs = 8;             // 한 웨이브 오브 개수
    float m_Radius = 140.f;         // 원형 반지름
    float m_Speed = 800.f;         // 발사 속도

private:
    void SpawnWave(CActor* owner);   // 한 웨이브(8방향) 스폰
};
