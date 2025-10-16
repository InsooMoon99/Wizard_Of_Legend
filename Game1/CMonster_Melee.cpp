#include "stdafx.h"
#include "CMonster_Melee.h"

#include "CGhoulLarge.h"

CMonster_Melee::CMonster_Melee()
{

}

CMonster_Melee::~CMonster_Melee()
{

}

void CMonster_Melee::OnStart(CMonsterBase* _Owner)
{
	m_delta = 0.f;
	m_moved = false;
	m_hit = false;
	m_done = false;

    SOUND->StopPlay(m_MeleeParams.AttackSound, 0.2f);
}

void CMonster_Melee::OnUpdate(CMonsterBase* _Owner, float _delta)
{
    m_delta += _delta;

    //if (!m_moved && m_delta >= m_moveStart)
    if (m_delta >= m_MeleeParams.moveStart && m_delta <= m_MeleeParams.moveEnd)
    {
    	_Owner->MoveTile(_Owner->GetTraceDir(), m_MeleeParams.moveDist);

        //m_moved = true;
    }

    if (!m_hit && m_delta >= m_MeleeParams.hitTime)
    {
        FSkillInfo info{};
        info.Instigator = _Owner;
        info.SpawnPos = _Owner->Get_Collision()->GetWorldPos() + _Owner->GetTraceDir() * 100.f;
        info.Damage = m_MeleeParams.damage;

        SKILL->Spawn_SkillActor(ESkillActor::AttackCollision, info, m_MeleeParams.hitBoxSize);

        m_hit = true;
    }

    if (m_delta >= m_MeleeParams.duration)
    {
        m_done = true;
    }
}

void CMonster_Melee::OnExit(CMonsterBase* _Owner)
{

}

