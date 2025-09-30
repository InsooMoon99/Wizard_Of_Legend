#include "stdafx.h"
#include "CFire_Dragon.h"

#include "CBossPawn.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CSkill.h"
#include "CStatusComponent.h"
#include "CTile.h"

CFire_Dragon::CFire_Dragon(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	Vec2 spawnOffset = m_Skill_Info.Aim_Dir * 50.f;
	m_WaveMode = m_Skill_Info.WaveMode;

	// Fire_Dragon Collision
	{
		m_Collision = make_shared<ObCircle>();
		m_Collision->scale = Vec2(80.f, 80.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x, m_Pos.y) + spawnOffset);
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::CIRCLE;
		m_Collision->isFilled = false;
		//m_Collision->rotation = m_Skill_Info.Angle;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);

		COLLISION->AddCollider(m_Collision.get());
	}

	m_FireDragonRight = RESOURCE->CreateSprite(L"Skill\\DragonArcRight.png", Vec2(37.f, 25.f) * 3.2f, Int2(1, 1), true);
	m_FireDragonRight->SetParentRT(*m_Collision);

	m_FireDragonLeft = RESOURCE->CreateSprite(L"Skill\\DragonArcLeft.png", Vec2(37.f, 25.f) * 3.2f, Int2(1, 1), true);
	m_FireDragonLeft->SetParentRT(*m_Collision);

	Vec2 fireSize = Vec2(768.f / 6.f, 512.f / 4.f) / 2.5f;
	m_FireDragon_Tail = RESOURCE->CreateSprite(L"Particle\\Fire_Particle_6.bmp", fireSize, Int2(6, 4), true);
	m_FireDragon_Tail->SetParentRT(*m_Collision);
	m_FireDragon_Tail->SetLocalPosX(-55.f);
	m_FireDragon_Tail->ChangeAnim(ANIMSTATE::LOOP, 0.15f);
	m_FireDragon_Tail->frame.y = 0;

	m_FireBall_Particle.resize(4);
}

CFire_Dragon::~CFire_Dragon()
{
	Release();
}

void CFire_Dragon::BeginPlay()
{
	Super::BeginPlay();

	if (m_Skill_Info.Aim_Dir.x > 0.f)
		m_CurFireImg = m_FireDragonRight;

	else
		m_CurFireImg = m_FireDragonLeft;

	SOUND->Stop("FireArc");
	SOUND->Play("FireArc");
	SOUND->SetVolume("FireArc", 0.1f);


	const Vec2 offset[4] = { { +60.f, -20.f },{ +60.f, -10.f },{ +60.f, +10.f },{ +60.f, +20.f } };
	Vec2 fwd = m_Skill_Info.Aim_Dir; fwd.Normalize();
	Vec2 right(fwd.y, -fwd.x);

	auto ToWorld = [&](const Vec2& local) { return m_Pos + fwd * local.x + right * local.y; };

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

	m_WavePhase = (m_WaveMode == 0) ? (PI * 0.5f) : (PI * 1.5f); 
	float amplitude = 30.f;  
	m_LastSinOffset = amplitude * sinf(m_WavePhase);

	int random = rand() % 50;
	m_RandomSpeed = 850.f + random;
}

void CFire_Dragon::Tick()
{
	Super::Tick();
	if (m_PendingKill) return;
	if (m_Collision) m_Pos = m_Collision->GetWorldPos();
	if (CheckHitWall()) return;

	m_SpawnLife += DELTA;
	if (m_SpawnLife >= 1.5f) { m_PendingKill = true; return; }

	Vec2 dir = m_Skill_Info.Aim_Dir; dir.Normalize();
	Vec2 right(dir.y, -dir.x);

	float amplitude = 40.f;  
	float frequency = 8.f;   

	float sinOffset = amplitude * sinf(m_WavePhase + m_SpawnLife * frequency);

	Vec2 delta = dir * (m_RandomSpeed * DELTA) + right * (sinOffset - m_LastSinOffset);
	m_Collision->MoveWorldPos(delta);
	m_LastSinOffset = sinOffset;

	if (delta.LengthSquared() > 1e-6f) 
	{
		float rot = atan2f(delta.y, delta.x);
		if (dir.x >= 0.f)
		{
			m_CurFireImg = m_FireDragonRight;
			m_CurFireImg->rotation = rot;
			m_CurFireImg->reverseLR = false;
		}
		else 
		{
			m_CurFireImg = m_FireDragonLeft;
			m_CurFireImg->rotation = rot + PI;
			m_CurFireImg->reverseLR = false;
		}
		m_FireDragon_Tail->rotation = rot;
	}

	m_Collision->Update();
	m_CurFireImg->Update();
	m_FireDragon_Tail->Update();
}


void CFire_Dragon::LateUpdate()
{
	Super::LateUpdate();

}

void CFire_Dragon::Render()
{
	Super::Render();

	m_Collision->Render();
	m_FireDragon_Tail->Render();
	m_CurFireImg->Render();

}

void CFire_Dragon::Release()
{
	Super::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
	m_FireDragonRight.reset();
	m_FireDragonLeft.reset();
	m_CurFireImg.reset();

}

bool CFire_Dragon::CheckHitWall()
{
	return Super::CheckHitWall();
}

void CFire_Dragon::OnHitWall(const Vec2& hitPos)
{
	PARTICLE->Spawn_ParticleDir(L"HitSpark", hitPos);
	PARTICLE->Spawn_ParticleDir(L"Fire_HitEffect2", hitPos);

	SOUND->Stop("FireArc_End");
	SOUND->Play("FireArc_End");
	SOUND->SetVolume("FireArc_End", 0.2f);

	m_PendingKill = true;
}

void CFire_Dragon::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentBeginOverlap(collider, other);

	CCharacter* hit = dynamic_cast<CCharacter*>(other->Get_Owner());
	if (!hit)return;

	// TODO : 팀킬/자기 무시
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

	// Particle
	{
		Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
		Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
		PARTICLE->Spawn_ParticleDir(L"HitSpark", particlePos + randomVec);
		PARTICLE->Spawn_ParticleDir(L"Fire_HitEffect2", particlePos + randomVec, Vec2(0.7f));

		SOUND->Stop("FireArc_End");
		SOUND->Play("FireArc_End");
		SOUND->SetVolume("FireArc_End", 0.2f);
	}
	
	m_PendingKill = true;
}

void CFire_Dragon::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);

}