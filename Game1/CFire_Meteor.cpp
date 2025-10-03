#include "stdafx.h"
#include "CFire_Meteor.h"

#include "CBossPawn.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CSkill.h"
#include "CStatusComponent.h"
#include "CTile.h"

CFire_Meteor::CFire_Meteor(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	// Fire_Ball Collision
	{
		m_Collision = make_shared<ObCircle>();
		m_Collision->scale = Vec2(200.f, 200.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x, m_Pos.y));
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::CIRCLE;
		m_Collision->isFilled = false;
		m_Collision->rotation = m_Skill_Info.Angle - PI/2;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);
	}

	m_FireMeteor = RESOURCE->CreateSprite(L"Skill\\MeteorStrike.png", Vec2(376.f / 4.f, 132.f) * 2.5f, Int2(4,1), true);
	m_FireMeteor->SetParentRT(*m_Collision);
	m_FireMeteor->SetLocalPos(Vec2(0.f, 40.f));
	m_FireMeteor->ChangeAnim(ANIMSTATE::LOOP, 0.1);

	m_bArmed = false;
	m_bAddedToMgr = false;
	bFirst = true;
	bCood = false;

	m_RemainDist = max(0.f, m_Skill_Info.TravelMax);
}

CFire_Meteor::~CFire_Meteor()
{
	Release();
}

void CFire_Meteor::BeginPlay()
{
	Super::BeginPlay();

	SOUND->Stop("FireBall");
	SOUND->Play("FireBall");
	SOUND->SetVolume("FireBall", 0.1f);

}

void CFire_Meteor::Tick()
{
	Super::Tick();

	if (m_PendingKill)
		return;

	// 위치 갱신
	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	// 이동 후, 벽 충돌 먼저 체크
	if (CheckHitWall()) 
		return;

	// Life Time
	m_SpawnLife += DELTA;

	if (m_SpawnLife >= 0.15f && bFirst)
	{
		bFirst = false;

		PARTICLE->Spawn_Particle(L"MeteorEffect", m_Collision->GetWorldPos());
	}

	float nextPos = m_Speed * DELTA;

	if (!m_bArmed && m_RemainDist <= m_ArmDist)
	{
		if (!m_bAddedToMgr)
		{
			COLLISION->AddCollider(m_Collision.get());
			m_bAddedToMgr = true;
		}
		m_bArmed = true;
	}

	if (m_RemainDist <= nextPos + 1e-4f)
	{
		if (!m_bAddedToMgr)
		{
			COLLISION->AddCollider(m_Collision.get());
			m_bAddedToMgr = true;
			m_bArmed = true;
		}

		Vec2 pos = m_Collision->GetWorldPos();

		OnHitWall(pos);

		bCood = true;
		return;
	}

	m_Collision->MoveWorldPos(m_Skill_Info.Aim_Dir * nextPos);
	m_RemainDist -= nextPos;

	m_Collision->Update();
	m_FireMeteor->Update();
}

void CFire_Meteor::LateUpdate()
{
	Super::LateUpdate();

	
}

void CFire_Meteor::Render()
{
	Super::Render();

	//if (m_PendingKill) return;

	m_Collision->Render();
	m_FireMeteor->Render();

}

void CFire_Meteor::Release()
{
	Super::Release();

	if (m_Collision)
	{             
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
	m_FireMeteor.reset();

}

bool CFire_Meteor::CheckHitWall()
{
	//return Super::CheckHitWall();

	return false;
}

void CFire_Meteor::	OnHitWall(const Vec2& hitPos)
{
	PARTICLE->Spawn_Particle(L"HitSpark", hitPos);
	PARTICLE->Spawn_Particle(L"Fire_HitEffect2", hitPos, Vec2(2.5f));

	SOUND->StopPlay("MeteorEx", 0.08f);

	m_PendingKill = true;
}

void CFire_Meteor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentBeginOverlap(collider, other);

	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	// 팀킬/자기 무시
	if (hit == m_Skill_Info.Instigator) return;
	//if (IsSameTeam(hit, m_Collision->Get_Instigator())) return;

	// 색 변경 : Hit Monster에서 인터페이스 상속받아야함
	IColorChanger* colorChanger = hit;
	if (colorChanger)
		colorChanger->Hit_Color();

	FHitDamage hitDamage;
	{
		Vec2 dir = hit->Get_WorldPos() - m_StartPos;
		if (dir.LengthSquared() > 1e-8f) dir.Normalize();

		hitDamage.Dir = dir;
		hitDamage.Damage = m_Skill_Info.Damage;
		hitDamage.colldier = collider;
		hitDamage.other = other;

		if (hit->IsKnockbackPrev())
		{
			hitDamage.Amount = m_Skill_Info.Amount;
			hitDamage.Duration = 0.25f;
		}
		else
		{
			hitDamage.Amount = 0.f;
			hitDamage.Duration = 0.f;
		}
	}

	hit->TakeDamage(hitDamage);

	// Particle
	{
		Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
		Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
		PARTICLE->Spawn_ParticleDir(L"HitSpark", particlePos + randomVec);
		PARTICLE->Spawn_ParticleDir(L"Fire_HitEffect2", particlePos + randomVec);

	}

	//m_PendingKill = true;
}

void CFire_Meteor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);


}
