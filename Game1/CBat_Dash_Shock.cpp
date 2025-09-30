#include "stdafx.h"
#include "CBat_Dash_Shock.h"

#include "CPlayer.h"

CBat_Dash_Shock::CBat_Dash_Shock(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;

	// �浹ü
	{
		m_Collision = make_shared<ObCircle>();
		m_Collision->scale = Vec2(240.f, 240.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x - 50.f, m_Pos.y));
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::CIRCLE;
		m_Collision->isFilled = false;
		//m_Collision->rotation = m_Skill_Info.Angle;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);

		COLLISION->AddCollider(m_Collision.get());
	}

	m_Shock = PARTICLE->Spawn_ParticleDir(L"Fist_Lightning_Y", m_Pos, Vec2(1.35f));

}

CBat_Dash_Shock::~CBat_Dash_Shock()
{
	Release();
}

void CBat_Dash_Shock::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	//SOUND->Stop("ImpactLightning");
	//SOUND->Play("ImpactLightning");
	//SOUND->SetVolume("ImpactLightning", 0.2f);

	SOUND->StopPlay("ImpactLightning", 0.2f);
}

void CBat_Dash_Shock::Tick()
{
	CSkill_Actor::Tick();

	if (CHelper::IsLastFrame(m_Shock))
	{
		m_PendingKill = true;
	}


	m_Collision->Update();
	m_Shock->Update();
}

void CBat_Dash_Shock::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CBat_Dash_Shock::Render()
{
	CSkill_Actor::Render();

	m_Collision->Render();
	m_Shock->Render();
}

void CBat_Dash_Shock::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
	m_Shock.reset();
}

void CBat_Dash_Shock::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	CPlayer* player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	FHitDamage hitStruct;
	hitStruct.Damage = m_Skill_Info.Damage;
	hitStruct.colldier = collider;
	hitStruct.other = other;

	player->TakeDamage(hitStruct);

	cout << "Shock Hit " << m_Skill_Info.Damage << endl;
}

void CBat_Dash_Shock::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}
