#include "stdafx.h"
#include "CWindSlash.h"

#include "CBossPawn.h"
#include "CStatusComponent.h"

CWindSlash::CWindSlash(FSkillInfo& _Info)
	: CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = _Info.SpawnPos;

	Vec2 pos_Offset = m_Skill_Info.Aim_Dir * 80.f;
	float rotation_Offset = - PI / 2;

	Vec2 pos = Vec2(m_Pos.x + pos_Offset.x, m_Pos.y + pos_Offset.y);

	m_Collision = make_shared<ObCircle>();
	m_Collision->scale = Vec2(80.f, 80.f);
	m_Collision->SetWorldPos(pos);
	m_Collision->visible = false;
	m_Collision->collider = COLLIDER::CIRCLE;
	m_Collision->isFilled = false;
	m_Collision->rotation = m_Skill_Info.Angle + rotation_Offset;
	m_Collision->Set_Owner(this);
	m_Collision->Set_Instigator(m_Skill_Info.Instigator);

	COLLISION->AddCollider(m_Collision.get());

	shared_ptr<ObImage> wind = PARTICLE->Spawn_ParticleDir(L"Wind_Slash", pos);
	wind->rotation = m_Collision->rotation;
}

CWindSlash::~CWindSlash()
{
	Release();
}

void CWindSlash::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	SOUND->Stop("WindSlash");
	SOUND->Play("WindSlash");
	SOUND->SetVolume("WindSlash", 0.1f);

}

void CWindSlash::Tick()
{
	CSkill_Actor::Tick();

	if (m_PendingKill) return;

	// 위치 갱신
	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	m_LifeTime += DELTA;

	if (m_LifeTime >= 0.1f)
	{
		m_PendingKill = true;
	}

	m_Collision->Update();
}

void CWindSlash::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CWindSlash::Render()
{
	CSkill_Actor::Render();

	if (m_PendingKill) return;

	m_Collision->Render();
}

void CWindSlash::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
}

void CWindSlash::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	if (hit == m_Skill_Info.Instigator) return;
	//if (IsSameTeam(hit, m_Collision->Get_Instigator())) return;

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

	// 파티클, 사운드
	{
		Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
		Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
		PARTICLE->Spawn_ParticleDir(L"HitSpark", Vec2(particlePos) + randomVec);

		SOUND->Stop("ImpactAir");
		SOUND->Play("ImpactAir");
		SOUND->SetVolume("ImpactAir", 0.1f);

		CAM->StartShake(5.f, 8.f, 0.12f);
	}

	//m_PendingKill = true;
}

void CWindSlash::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}