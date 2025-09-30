#include "stdafx.h"
#include "CFire_Ball.h"

#include "CBossPawn.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CSkill.h"
#include "CStatusComponent.h"
#include "CTile.h"

CFire_Ball::CFire_Ball(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	// Fire_Ball Collision
	{
		m_Collision = make_shared<ObCircle>();
		m_Collision->scale = Vec2(80.f, 80.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x, m_Pos.y));
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::CIRCLE;
		m_Collision->isFilled = false;
		m_Collision->rotation = m_Skill_Info.Angle;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);

		COLLISION->AddCollider(m_Collision.get());
	}

	m_FireBall = RESOURCE->CreateSprite(L"Skill\\FireBall.png", Vec2(980.f / 14.f, 50.f) * 2.5f, Int2(14,1), true);
	m_FireBall->SetParentRT(*m_Collision);
	m_FireBall->SetLocalPosX(10.f);
	m_FireBall->ChangeAnim(ANIMSTATE::ONCE, 0.05f);

	m_FireBall_Particle.resize(4);

}

CFire_Ball::~CFire_Ball()
{
	Release();
}

void CFire_Ball::BeginPlay()
{
	Super::BeginPlay();

	SOUND->Stop("FireBall");
	SOUND->Play("FireBall");
	SOUND->SetVolume("FireBall", 0.1f);

	const Vec2 offset[4] =
	{
		{ +40.f,  -20.f },  
		{ +40.f,  -10.f }, 
		{ +40.f, +10.f }, 
		{ +40.f, +20.f }, 
	};

	Vec2 fwd = m_Skill_Info.Aim_Dir; fwd.Normalize();
	Vec2 right(fwd.y, -fwd.x);

	auto ToWorld = [&](const Vec2& local) {
		return m_Pos + fwd * local.x + right * local.y;
		};

	for (int i = 0; i < 4; ++i)
	{
		wstring key;

		switch (i)
		{
		case 0: key = L"Fire1";  break;
		case 1: key = L"Fire2"; break;
		case 2: key = L"Fire3"; break;
		case 3: key = L"Fire4"; break;
		}

		Vec2 worldPos = ToWorld(offset[i]);

		auto particle = PARTICLE->Spawn_ParticleDir(key, worldPos);
		if (!particle) continue;

		particle->rotation = m_Skill_Info.Angle; 
		particle->visible = true;

		m_FireBall_Particle[i] = particle; 
	}

}

void CFire_Ball::Tick()
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

	if (m_SpawnLife >= 1.5f)
	{
		m_PendingKill = true;
		return;
	}

	m_Collision->MoveWorldPos(m_Skill_Info.Aim_Dir * 1000.f * DELTA);

	m_Collision->Update();
	m_FireBall->Update();


}

void CFire_Ball::LateUpdate()
{
	Super::LateUpdate();

	
}

void CFire_Ball::Render()
{
	Super::Render();

	//if (m_PendingKill) return;

	m_Collision->Render();
	m_FireBall->Render();

}

void CFire_Ball::Release()
{
	Super::Release();

	if (m_Collision)
	{             
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
	m_FireBall.reset();

}

bool CFire_Ball::CheckHitWall()
{
	return Super::CheckHitWall();
}

void CFire_Ball::OnHitWall(const Vec2& hitPos)
{
	PARTICLE->Spawn_ParticleDir(L"HitSpark", hitPos);
	PARTICLE->Spawn_ParticleDir(L"Fire_HitEffect2", hitPos);

	SOUND->Stop("FireBall_End");
	SOUND->Play("FireBall_End");
	SOUND->SetVolume("FireBall_End", 0.15f);

	m_PendingKill = true;
}

void CFire_Ball::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
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

		SOUND->Stop("FireBall_End");
		SOUND->Play("FireBall_End");
		SOUND->SetVolume("FireBall_End", 0.15f);
	}

	m_PendingKill = true;
}

void CFire_Ball::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);


}
