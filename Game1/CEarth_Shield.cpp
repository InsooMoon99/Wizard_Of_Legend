#include "stdafx.h"
#include "CEarth_Shield.h"

#include "CBossPawn.h"
#include "CStatusComponent.h"

CEarth_Shield::CEarth_Shield(FSkillInfo& _Info)
	: CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = _Info.SpawnPos;

	m_Armor.resize(EARTH_SHIELD);
	m_Armor_Img.resize(EARTH_SHIELD);

	for (int i = 0; i < EARTH_SHIELD; ++i)
	{
		m_Angle[i] = i * (PI / 4) - (PI / 2);
	}

}

CEarth_Shield::~CEarth_Shield()
{
	Release();
}

void CEarth_Shield::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	SOUND->Play("Earth_Shield");
	SOUND->SetVolume("Earth_Shield", 0.85f);

	for (int i = 0; i < EARTH_SHIELD; ++i)
	{
		m_Armor[i] = make_shared<ObCircle>();
		m_Armor[i]->scale = Vec2(55.f, 55.f);
		m_Armor[i]->SetParentRT(*m_Skill_Info.Instigator->Get_Collision());
		m_Armor[i]->isFilled = false;
		m_Armor[i]->visible = false;
		m_Armor[i]->collider = COLLIDER::CIRCLE;
		m_Armor[i]->Set_Owner(this);
		m_Armor[i]->SetLocalPos(Vec2(cosf(m_Angle[i]) * m_Radius, sinf(m_Angle[i]) * m_Radius));

		m_Armor_Img[i] = RESOURCE->CreateSprite(L"Skill\\GAIA_ARMOR.bmp", Vec2(600.f / 12.f) * 1.6f, Int2(12, 1), true);
		m_Armor_Img[i]->ChangeAnim(ANIMSTATE::REVERSE_LOOP, 0.15f, true);
		m_Armor_Img[i]->SetParentRT(*m_Armor[i]);

		COLLISION->AddCollider(m_Armor[i].get());

		switch (i)
		{
		case 0:
		case 1:
		case 2:
			m_Armor_Img[i]->frame.x = i;
			break;
		case 3:
			m_Armor_Img[i]->frame.x = 4;
			break;
		case 4:
			m_Armor_Img[i]->frame.x = 6;
			break;
		case 5:
			m_Armor_Img[i]->frame.x = 7;
			break;
		case 6:
			m_Armor_Img[i]->frame.x = 9;
			break;
		case 7:
			m_Armor_Img[i]->frame.x = 10;
			break;
		}
	}


}

void CEarth_Shield::Tick()
{
	CSkill_Actor::Tick();

	if (m_PendingKill) return;

	// 위치 갱신
	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	m_SpawnTime -= DELTA;

	if (m_SpawnTime > 0.6f)
	{
		for (int i = 0; i < EARTH_SHIELD; ++i)
		{
			m_Angle[i] -= 3.5f * DELTA;

			m_Armor[i]->SetLocalPos(Vec2(cosf(m_Angle[i]) * m_Radius,
				sinf(m_Angle[i]) * m_Radius));

		}
	}

	else if (m_SpawnTime < 0.6f && m_SpawnTime > 0.f)
	{
		for (auto& img : m_Armor_Img)
			img->ChangeAnim(ANIMSTATE::STOP, 0.f);

		for (auto& armor : m_Armor)
			COLLISION->RemoveCollider(armor.get());
	}

	else if (m_SpawnTime <= 0.f)
	{
		SOUND->Play("Earth_Shield");
		SOUND->SetVolume("Earth_Shield", 0.85f);

		for (auto& armor : m_Armor)
		{
			PARTICLE->Spawn_ParticleDir(L"Stone3", armor->GetWorldPos());
		}

		m_PendingKill = true;

		return;
	}

	for (auto& img : m_Armor_Img)
		img->Update();

	for (auto& armor : m_Armor)
		armor->Update();

}

void CEarth_Shield::LateUpdate()
{
	CSkill_Actor::LateUpdate();


}

void CEarth_Shield::Render()
{
	CSkill_Actor::Render();

	if (m_PendingKill) return;

	for (auto& img : m_Armor_Img)
		img->Render();

	for (auto& armor : m_Armor)
	{
		armor->Render();
	}
}

void CEarth_Shield::Release()
{
	CSkill_Actor::Release();

	m_Armor.clear();
	m_Armor_Img.clear();

}

void CEarth_Shield::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
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
	SOUND->StopPlay("ImpactEarth", 0.12f);

	Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
	Vec2 particlePos = (collider->GetWorldPos() + other->GetWorldPos()) / 2.0f;
	PARTICLE->Spawn_ParticleDir(L"HitSpark", Vec2(particlePos) + randomVec);

	//m_PendingKill = true;
}

void CEarth_Shield::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);


}
