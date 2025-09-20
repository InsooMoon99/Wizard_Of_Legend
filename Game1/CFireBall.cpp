#include "stdafx.h"
#include "CFireBall.h"
#include "CResourceManager.h"
#include "CCharacter.h"
#include "CSkill.h"
#include "CStatusComponent.h"
#include "CTile.h"

CFireBall::CFireBall(SkillInfo& _Info) : CSkill_Actor(_Info)
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

CFireBall::~CFireBall()
{
	Release();
}

void CFireBall::BeginPlay()
{
	Super::BeginPlay();

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
		case 0: key = L"Fire4";  break;
		case 1: key = L"Fire1"; break;
		case 2: key = L"Fire3"; break;
		case 3: key = L"Fire2"; break;
		}

		Vec2 worldPos = ToWorld(offset[i]);

		auto particle = PARTICLE->Spawn_Particle(key, worldPos);
		if (!particle) continue;

		particle->rotation = m_Skill_Info.Angle; 
		particle->visible = true;

		m_FireBall_Particle[i] = particle; 
	}

}

void CFireBall::Tick()
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

void CFireBall::LateUpdate()
{
	Super::LateUpdate();

	
}

void CFireBall::Render()
{
	Super::Render();

	//if (m_PendingKill) return;

	m_Collision->Render();
	m_FireBall->Render();


}

void CFireBall::Release()
{
	Super::Release();

	if (m_Collision)
	{             
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
	m_FireBall.reset();
}

bool CFireBall::CheckHitWall()
{
	if (!m_Collision) return false;

	const Vec2 fireball_Col = m_Collision->GetWorldPos();
	const float radius = m_Collision->scale.x * 0.5f;
	
	const Vec2 min = fireball_Col - Vec2(radius, radius);
	const Vec2 max = fireball_Col + Vec2(radius, radius);

	vector<CTile*> canDir;
	TILE->QueryTilesAABB(min, max, canDir);

	for (CTile* tile : canDir)
	{
		if (tile->Get_DrawID() != (int)ETileType::WALL)
			continue;

		auto tileCol = tile->Get_Collision().get();
		tileCol->SetWorldPos(tile->Get_WorldPos());

		if (m_Collision->Intersect(tileCol))
		{
			Vec2 pos;
			pos.x = (tile->Get_WorldPos().x + m_Collision->GetWorldPos().x) / 2.f;
			//pos.y = (tile->Get_WorldPos().y + m_Collision->GetWorldPos().y) / 2.f;
			pos.y = m_Collision->GetWorldPos().y;

			OnHitWall(pos);
			return true;
		}
	}
	return false;
}

void CFireBall::OnHitWall(const Vec2& hitPos)
{
	PARTICLE->Spawn_Particle(L"HitSpark", hitPos);
	PARTICLE->Spawn_Particle(L"Fire_HitEffect", hitPos);

	m_PendingKill = true;
}

void CFireBall::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
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
	PARTICLE->Spawn_Particle(L"Fire_HitEffect", collider->GetWorldPos());

	m_PendingKill = true;
}

void CFireBall::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);


}
