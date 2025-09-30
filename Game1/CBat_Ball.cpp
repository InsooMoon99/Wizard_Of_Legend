#include "stdafx.h"
#include "CBat_Ball.h"

#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CPlayer.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"

CBat_Ball::CBat_Ball(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	// 충돌체
	{
		m_Collision = make_shared<ObCircle>();
		m_Collision->scale = Vec2(40.f, 40.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x - 50.f, m_Pos.y));
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::CIRCLE;
		m_Collision->isFilled = false;
		//m_Collision->rotation = m_Skill_Info.Angle;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);

		COLLISION->AddCollider(m_Collision.get());
	}
	
	m_Batball = RESOURCE->CreateSprite(L"Monster\\Lightning\\MetalOrb.png", Vec2(128.f / 4.f, 32.f) * 3.f, Int2(4, 1), true);
	m_Batball->SetParentRT(*m_Collision);
	m_Batball->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
}

CBat_Ball::~CBat_Ball()
{
	Release();
}

void CBat_Ball::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	m_Start = m_Pos;
	m_Target = m_Skill_Info.Target ? m_Skill_Info.Target->Get_WorldPos() : m_Pos;

	Vec2 r = m_Target - m_Start;
	Vec2 g = Vec2(0.f, m_GravityY);
	float T = m_FlightTime;

	m_V0 = (r - 0.5f * g * T * T) / T;

	m_Arrived = false;
	m_Timer = 0.f;

	PARTICLE->Spawn_ParticleDir(L"ShockSpark_AS", m_Pos, Vec2(1.3f, 1.5f));
}

void CBat_Ball::Tick()
{
	CSkill_Actor::Tick();
	if (m_Arrived) return;

	m_Timer += DELTA;
	float t = min(m_Timer, m_FlightTime);

	Vec2 g = Vec2(0.f, m_GravityY);
	Vec2 pos = m_Start + m_V0 * t + 0.5f * g * t * t;

	m_Collision->SetWorldPos(pos);
	m_Collision->Update();

	if (m_Timer >= m_FlightTime)
	{
		m_Collision->SetWorldPos(m_Target);
		m_Arrived = true;

		//COLLISION->RemoveCollider(m_Collision.get());
		// m_Collision->visible = false;

		auto owner = dynamic_cast<CBossPawn*>(m_Skill_Info.Instigator);
		if (!owner) return;

		owner->GetController()->OnDashStart.Broadcast(m_Collision->GetWorldPos());
		//owner->GetController()->SetBallMovePos(m_Collision->GetWorldPos());

	}

	m_Batball->Update();
}

void CBat_Ball::LateUpdate()
{
	CSkill_Actor::LateUpdate();

}

void CBat_Ball::Render()
{
	CSkill_Actor::Render();

	m_Collision->Render();
	m_Batball->Render();
}

void CBat_Ball::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}

	m_Batball.reset();
}

bool CBat_Ball::CheckHitWall()
{
	return CSkill_Actor::CheckHitWall();

}

void CBat_Ball::OnHitWall(const Vec2& hitPos)
{
	CSkill_Actor::OnHitWall(hitPos);

}

void CBat_Ball::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	if (m_Arrived)
	{
		CBossPawn* bossPawn = dynamic_cast<CBossPawn*>(hit);
		if (bossPawn && bossPawn != m_Skill_Info.Instigator)
		{
			Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
			Vec2 particlePos = (m_Collision->GetWorldPos());
			PARTICLE->Spawn_ParticleDir(L"HitSpark", particlePos + randomVec, Vec2(0.6f));

			m_PendingKill = true;
		}

		return;
	}

	CPlayer* player = dynamic_cast<CPlayer*>(hit);
	if (!player) return;
	// 팀킬/자기 무시, 그런데 얘는 충돌이 Girl이랑 돼야 함
	//if (hit == m_Skill_Info.Instigator) return;
	//if (IsSameTeam(hit, m_Collision->Get_Instigator())) return;

	if (m_HitObjects.find(other) != m_HitObjects.end())
		return;

	m_HitObjects.insert(other);

	if (player->IsKnockback())
	{
		Vec2 dir = player->Get_WorldPos() - m_StartPos;
		dir.Normalize();
		player->Get_Collision()->MoveWorldPos(dir * m_Skill_Info.Amount * DELTA);
	}

	FHitDamage hitDamage;
	hitDamage.Damage = m_Skill_Info.Damage;
	hitDamage.colldier = collider;
	hitDamage.other = other;

	player->TakeDamage(hitDamage);

	cout << "Ball Hit " << m_Skill_Info.Damage << endl;
}

void CBat_Ball::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}
