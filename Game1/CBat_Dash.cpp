#include "stdafx.h"
#include "CBat_Dash.h"

#include "CBat_Dash_Shock.h"
#include "CBossPawn.h"
#include "CBoss_TwinMan.h"
#include "CSkillManager.h"

CBat_Dash::CBat_Dash(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = m_Pos;
	 
	// 충돌체
	{
		m_Collision = make_shared<ObRect>();
		m_Collision->scale = Vec2(200.f, 200.f);
		m_Collision->SetWorldPos(Vec2(m_Pos.x - 50.f, m_Pos.y));
		m_Collision->visible = false;
		m_Collision->collider = COLLIDER::RECT;
		m_Collision->isFilled = false;
		m_Collision->Set_Owner(this);
		m_Collision->Set_Instigator(m_Skill_Info.Instigator);

		COLLISION->AddCollider(m_Collision.get());
	}

	Vec2 dashEffectSize = Vec2(40.f * 1.2f, 66.f) * 2.f;
	m_DashEffect = RESOURCE->CreateSprite(L"Monster\\Lightning\\LightningChainDashEffect0.png", dashEffectSize, Int2(1, 1), true);
	m_DashEffect->SetParentRT(*m_Collision);
	m_DashEffect->ChangeAnim(ANIMSTATE::STOP, 0.f);

	m_DashEffect2 = PARTICLE->Spawn_ParticleDir(L"Light_Dash_Y", m_Pos);
}

CBat_Dash::~CBat_Dash()
{
	Release();
}

void CBat_Dash::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	m_Pawn = dynamic_cast<CBossPawn*>(m_Skill_Info.Instigator);

	Vec2 currentPos = m_Pawn->Get_Collision()->GetWorldPos();
	Vec2 toTarget = m_Skill_Info.TargetPos - currentPos;
	m_TargetDistance = toTarget.Length();
	toTarget.Normalize();
	m_DashDir = toTarget;

	m_DashEffect->rotation = Utility::DirToRadian(m_DashDir) - PI/2;
	m_DashEffect2->rotation = Utility::DirToRadian(m_DashDir) - PI / 2;
}

void CBat_Dash::Tick()
{
	CSkill_Actor::Tick();

	//if (IsDestroy()) return;

	Vec2 currentPos = m_Pawn->Get_Collision()->GetWorldPos();
	Vec2 toTarget = m_Skill_Info.TargetPos - currentPos;

	if (toTarget.Length() <= 20.f ||  toTarget.Dot(m_DashDir) < 0)
	{
		m_Pawn->Get_Collision()->SetWorldPos(m_Skill_Info.TargetPos);
		
		m_Skill_Info.SpawnPos = m_Skill_Info.TargetPos;
		SKILL->Spawn_SkillActor(ESkillActor::Dash_Shock, m_Skill_Info);

		m_PendingKill = true;

		return;
	}

	m_Pawn->Get_Collision()->MoveWorldPos(m_DashDir * 2500.f * DELTA);
	m_DashEffect->SetWorldPos(m_Pawn->Get_Collision()->GetWorldPos());

	m_Collision->Update();
	m_DashEffect->Update();
}

void CBat_Dash::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CBat_Dash::Render()
{
	CSkill_Actor::Render();

	m_DashEffect->Render();
	m_Collision->Render();
}

void CBat_Dash::Release()
{
	CSkill_Actor::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());

		m_Collision.reset();
	}
	m_DashEffect.reset();
}

bool CBat_Dash::CheckHitWall()
{
	return CSkill_Actor::CheckHitWall();
}

void CBat_Dash::OnHitWall(const Vec2& hitPos)
{
	CSkill_Actor::OnHitWall(hitPos);

}

void CBat_Dash::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);

	if (dynamic_cast<CPlayer*>(other->Get_Owner()) == nullptr)
		return;

	auto boss = dynamic_cast<CBoss_TwinMan*>(other->Get_Owner());
	if (boss) return;

	m_PendingKill = true;
}

void CBat_Dash::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);

}
