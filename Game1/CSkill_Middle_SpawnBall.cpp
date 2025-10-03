#include "stdafx.h"
#include "CSkill_Middle_SpawnBall.h"
#include "CBat_Dash.h"
#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CCharacter.h"
#include "CMiddle_SpawnBall.h"
#include "CStateComponent.h"

CSkill_Middle_SpawnBall::CSkill_Middle_SpawnBall()
{

}

CSkill_Middle_SpawnBall::~CSkill_Middle_SpawnBall()
{

}

void CSkill_Middle_SpawnBall::BeginPlay()
{
	CSkill::BeginPlay();

}

void CSkill_Middle_SpawnBall::Tick()
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

void CSkill_Middle_SpawnBall::LateUpdate()
{
	CSkill::LateUpdate();

}

void CSkill_Middle_SpawnBall::Render()
{
	CSkill::Render();

	for (auto& batBall : m_Ball)
	{
		if (!batBall->IsDestroy())
			batBall->Render();
	}
}

void CSkill_Middle_SpawnBall::Release()
{
	CSkill::Release();

}

bool CSkill_Middle_SpawnBall::CanActivate(const FSkillInfo& _Info)
{
	if (!CSkill::CanActivate(_Info))
		return false;

	// TODO : 보스는 굳이 이거 판단해야 하나?

	return true;

}

void CSkill_Middle_SpawnBall::Activate(CActor* _Owner)
{
    CSkill::Activate(_Owner);

    CCharacter* character = dynamic_cast<CCharacter*>(_Owner);
    if (!character) return;

    auto state = CHelper::GetComponent<CStateComponent>(character);
    auto status = CHelper::GetComponent<CStatusComponent>(character);
    if (!state || !status) return;

    const int   kNum = 5;
    const float radius = 140.f;               
    const float step = (2.f * PI) / kNum;

    Vec2  center = _Owner->Get_WorldPos();
    auto  ownerCol = _Owner->Get_Collision();

    for (int i = 0; i < kNum; ++i)
    {
        float ang = i * step;
        Vec2  offset = Vec2(cosf(ang), sinf(ang)) * radius;

        m_Skill_Info.Instigator = _Owner;
        m_Skill_Info.Target = _Owner;        
        m_Skill_Info.SpawnPos = center;      
        m_Skill_Info.Aim_Dir = Vec2(1.f, 0.f);
        m_Skill_Info.Angle = ang;
        m_Skill_Info.Damage = 10.f;

        auto spawn = make_shared<CMiddle_SpawnBall>(m_Skill_Info);
        spawn->BeginPlay();

        if (ownerCol)
        {
            if (auto sc = spawn->Get_Collision())
            {
                sc->SetParentRT(*ownerCol);
                sc->SetLocalPos(offset);
            }
        }

        m_Ball.emplace_back(spawn);
    }
}

