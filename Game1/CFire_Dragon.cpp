#include "stdafx.h"
#include "CFire_Dragon.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CSkill.h"
#include "CStatusComponent.h"
#include "CTile.h"

CFire_Dragon::CFire_Dragon(SkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	Vec2 spawnOffset = m_Skill_Info.Aim_Dir * 50.f;

	// Fire_Dragon Collision
	{
		m_Collision = make_shared<ObCircle>();
		m_Collision->scale = Vec2(80.f, 80.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x, m_Pos.y) + spawnOffset);
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::CIRCLE;
		m_Collision->isFilled = false;
		m_Collision->rotation = m_Skill_Info.Angle;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);

		COLLISION->AddCollider(m_Collision.get());
	}

	m_FireDragon = RESOURCE->CreateSprite(L"Skill\\DragonArcRight.png", Vec2(37.f, 25.f) * 2.5f, Int2(1, 1), true);
	m_FireDragon->SetParentRT(*m_Collision);

	m_FireBall_Particle.resize(4);

}

CFire_Dragon::~CFire_Dragon()
{
	Release();
}

void CFire_Dragon::BeginPlay()
{
	Super::BeginPlay();

	const Vec2 offset[4] =
	{
		{ +60.f,  -20.f },
		{ +60.f,  -10.f },
		{ +60.f, +10.f },
		{ +60.f, +20.f },
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

		auto particle = PARTICLE->Spawn_Particle(key, worldPos);
		if (!particle) continue;

		particle->rotation = m_Skill_Info.Angle;
		particle->visible = true;

		m_FireBall_Particle[i] = particle;
	}

}

void CFire_Dragon::Tick()
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

	m_Collision->MoveWorldPos(m_Skill_Info.Aim_Dir * 850.f * DELTA);

	m_Collision->Update();
	m_FireDragon->Update();
}

void CFire_Dragon::LateUpdate()
{
	Super::LateUpdate();

}

void CFire_Dragon::Render()
{
	Super::Render();

	m_Collision->Render();
	m_FireDragon->Render();

}

void CFire_Dragon::Release()
{
	Super::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
	m_FireDragon.reset();

}

bool CFire_Dragon::CheckHitWall()
{
	return Super::CheckHitWall();
}

void CFire_Dragon::OnHitWall(const Vec2& hitPos)
{
	PARTICLE->Spawn_Particle(L"HitSpark", hitPos);
	PARTICLE->Spawn_Particle(L"Fire_HitEffect2", hitPos);

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

	Vec2 dir = hit->Get_WorldPos() - m_StartPos;
	dir.Normalize();
	hit->Get_Collision()->MoveWorldPos(dir * m_Skill_Info.Amount * DELTA);

	// 색 변경 : Hit Monster에서 인터페이스 상속받아야함
	IColorChanger* colorChanger = hit;
	if (colorChanger)
		colorChanger->Hit_Color();

	auto status = CHelper::GetComponent<CStatusComponent>(hit);
	if (status)
		status->SetHp(status->GetHp() - m_Skill_Info.Damage);

	// Particle
	Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
	Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
	PARTICLE->Spawn_Particle(L"HitSpark", particlePos + randomVec);
	PARTICLE->Spawn_Particle(L"Fire_HitEffect2", particlePos + randomVec, 0.7f);

	m_PendingKill = true;
}

void CFire_Dragon::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);

}
