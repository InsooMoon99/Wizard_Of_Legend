#include "stdafx.h"
#include "CSkill_Middle_Ball.h"
#include "CBat_Dash.h"
#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CCharacter.h"
#include "CStateComponent.h"

CSkill_Middle_Ball::CSkill_Middle_Ball()
{

}

CSkill_Middle_Ball::~CSkill_Middle_Ball()
{

}

void CSkill_Middle_Ball::BeginPlay()
{
	CSkill::BeginPlay();

}

void CSkill_Middle_Ball::Tick()
{
	CSkill::Tick();

	for (auto iter = m_Ball.begin(); iter != m_Ball.end(); )
	{
		auto dash = *iter;

		dash->Tick();

		if (dash->IsDestroy())
			iter = m_Ball.erase(iter);

		else
			++iter;
	}

}

void CSkill_Middle_Ball::LateUpdate()
{
	CSkill::LateUpdate();

}

void CSkill_Middle_Ball::Render()
{
	CSkill::Render();

	for (auto& batBall : m_Ball)
	{
		if (!batBall->IsDestroy())
			batBall->Render();
	}
}

void CSkill_Middle_Ball::Release()
{
	CSkill::Release();

}

bool CSkill_Middle_Ball::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;

	// TODO : 보스는 굳이 이거 판단해야 하나?

	return true;

}

void CSkill_Middle_Ball::Activate(CActor* _Owner)
{
    CSkill::Activate(_Owner);

    CCharacter* character = dynamic_cast<CCharacter*>(_Owner);
    if (!character) return;

    auto state = CHelper::GetComponent<CStateComponent>(character);
    auto status = CHelper::GetComponent<CStatusComponent>(character);
    if (!state || !status) return;

    const int   ringCount = 3;
    const int   perRing = 8;
    const float baseRadius = 100.f;     // 첫 링 반지름
    const float gap = 100.f;     // 링 간격(요청)
    const float baseOmega = 1.8f;      // 기본 각속도(라디안/초)
    const float step = (2.f * PI) / perRing;

    Vec2  center = _Owner->Get_WorldPos();
    auto  ownerCol = _Owner->Get_Collision();

    for (int r = 0; r < ringCount; ++r)
    {
        float radius = baseRadius + gap * r;

        // 링마다 속도/방향 약간씩 다르게 (가운데 링 반시계→시계)
        float omega = baseOmega + 0.2f * r;     // 바깥일수록 조금 빠르게
        if (r == 1) omega = -omega;              // 가운데 링은 반대 방향

        // 링마다 시작 위상 살짝 오프셋(겹침 방지)
        float phase = (r * 0.25f) * step;

        for (int i = 0; i < perRing; ++i)
        {
            float ang = i * step + phase;
            Vec2  offset = Vec2(cosf(ang), sinf(ang)) * radius;

            m_Skill_Info.Instigator = _Owner;
            m_Skill_Info.Target = _Owner;      
            m_Skill_Info.SpawnPos = center;
            m_Skill_Info.Aim_Dir = Vec2(1.f, 0.f);
            m_Skill_Info.Angle = ang;
            m_Skill_Info.Damage = 10.f;

            auto orb = make_shared<CMiddle_Ball>(m_Skill_Info);
            orb->BeginPlay();

            if (ownerCol)
            {
                orb->EnableOrbit(radius, ang, omega, ownerCol.get());
            }

            m_Ball.emplace_back(orb);
        }
    }
}
