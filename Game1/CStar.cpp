#include "stdafx.h"
#include "CStar.h"

#include "CBossPawn.h"
#include "CBoss_Controller.h"
#include "CStar_Collision.h"

CStar::CStar(FSkillInfo& _Info) : CSkill_Actor(_Info)
{
	m_Pos = _Info.TargetPos;
	m_StartPos = m_Pos;

	m_Circles.resize(STAR_CIRCLE);
	m_CircleImg.resize(STAR_CIRCLE);
	m_OrbShadow.resize(STAR_CIRCLE);
	m_StartInfo.resize(STAR_CIRCLE);
	m_SecondInfo.resize(STAR_CIRCLE);
	m_ThirdInfo.resize(STAR_CIRCLE);

	float angle = (2.f * PI) / STAR_CIRCLE;
	float offset_Angle = PI / 10.f;

	Vec2 imgSize = Vec2(216.f / 9.f, 56.f) * 3.f;

	for (int i = 0; i < STAR_CIRCLE; i++)
	{
		m_Circles[i] = make_shared<ObCircle>();
		m_Circles[i]->scale = Vec2(30.f, 30.f);
		m_Circles[i]->visible = false;
		m_Circles[i]->isFilled = false;
		m_Circles[i]->Set_Owner(this);
		m_Circles[i]->Set_Instigator(m_Skill_Info.Instigator);
		m_Circles[i]->collider = COLLIDER::CIRCLE;

		Vec2 pos = Vec2(m_Pos.x + cos(i * angle + offset_Angle) * 180.f, m_Pos.y + sin(i * angle + offset_Angle) * 180.f);
		m_Circles[i]->SetLocalPos(Vec2(pos));

		COLLISION->AddCollider(m_Circles[i].get());

		m_CircleImg[i] = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Girl_Orb.png", imgSize, Int2(9, 1), true);
		m_CircleImg[i]->SetParentRT(*m_Circles[i]);
		m_CircleImg[i]->SetLocalPosY(-20.f);
		m_CircleImg[i]->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_OrbShadow[i] = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(75.f) * 0.75f, true);
		m_OrbShadow[i]->SetParentRT(*m_Circles[i]);
		m_OrbShadow[i]->SetLocalPosY(-40.f);
		m_OrbShadow[i]->color = Color(0.5f, 0.5f, 0.5f, 0.25f);

		// start
		{
			Vec2 dir = m_Pos - pos;
			dir.Normalize();

			m_StartInfo[i].SpawnPos = pos;
			m_StartInfo[i].Aim_Dir = dir;
			m_StartInfo[i].Angle = Utility::DirToRadian(dir);
		}

		// second
		{
			Vec2 secondPos = Vec2(
				m_Pos.x + cos(((i+2) % STAR_CIRCLE) * angle + offset_Angle) * 180.f, 
				m_Pos.y + sin(((i+2) % STAR_CIRCLE) * angle + offset_Angle) * 180.f);

			Vec2 dir = secondPos - pos;
			dir.Normalize();

			m_SecondInfo[i].SpawnPos = pos;
			m_SecondInfo[i].Aim_Dir = dir;
			m_SecondInfo[i].Angle = Utility::DirToRadian(dir);

		}

		// third
		{
			Vec2 secondPos = Vec2(
				m_Pos.x + cos(((i + 1) % STAR_CIRCLE) * angle + offset_Angle) * 180.f,
				m_Pos.y + sin(((i + 1) % STAR_CIRCLE) * angle + offset_Angle) * 180.f);

			Vec2 dir = secondPos - pos;
			dir.Normalize();

			m_ThirdInfo[i].SpawnPos = pos;
			m_ThirdInfo[i].Aim_Dir = dir;
			m_ThirdInfo[i].Angle = Utility::DirToRadian(dir);
		}
		
	}

	bFirst = true;
}

CStar::~CStar()
{
	Release();
}

void CStar::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	m_BossController = dynamic_cast<CBossPawn*>(m_Skill_Info.Instigator)->GetController();

}

void CStar::Tick()
{
	CSkill_Actor::Tick();

	if (m_CircleImg[0]->frame.x == 7 && bFirst)
	{
		m_BossController->OnStarStart.Broadcast(m_Pos);

		for (int i = 0; i < STAR_CIRCLE; ++i)
		{
			auto skill = SKILL->Spawn_SkillActor(ESkillActor::Star_Collision, m_StartInfo[i], EStarType::First);
			m_Spawned.push_back(skill);
		}

		bFirst = false;
	}

	if (bFirst == false)
	{
		m_Attack_Timer += DELTA;

		if (bSecond && m_Attack_Timer >= 1.f)
		{
			for (int i = 0; i < STAR_CIRCLE; ++i)
			{
				SOUND->StopPlay("ImpactLightning", 0.15f);
				auto skill = SKILL->Spawn_SkillActor(ESkillActor::Star_Collision, m_SecondInfo[i], EStarType::Second);
				m_Spawned.push_back(skill);
			}

			bSecond = false;
			m_Attack_Timer = 0.f;
		}
	}

	if (bSecond == false)
	{
		m_Attack_Timer += DELTA;

		if (bThird && m_Attack_Timer >= 1.f)
		{
			for (int i = 0; i < STAR_CIRCLE; ++i)
			{
				SOUND->StopPlay("ImpactLightning", 0.15f);
				auto skill = SKILL->Spawn_SkillActor(ESkillActor::Star_Collision, m_ThirdInfo[i], EStarType::Third);
				m_Spawned.push_back(skill);
			}

			bThird = false;
			m_Attack_Timer = 0.f;
		}
	}

	if (bThird == false)
	{
		m_Attack_Timer += DELTA;

		if (m_Attack_Timer >= 1.5)
		{
			m_BossController->ChangeState(ECharacterState::Idle);

			m_PendingKill = true;
		}
	}

	for (auto& circle : m_Circles)
		circle->Update();

	for (auto& circle : m_CircleImg)
		circle->Update();

	for (auto& shadow : m_OrbShadow)
		shadow->Update();

}

void CStar::LateUpdate()
{
	CSkill_Actor::LateUpdate();
}

void CStar::Render()
{
	CSkill_Actor::Render();

	for (auto& circle : m_Circles)
		circle->Render();

	for (auto& shadow : m_OrbShadow)
		shadow->Render();

	for (auto& circle_Img : m_CircleImg)
		circle_Img->Render();

}

void CStar::Release()
{
	CSkill_Actor::Release();

	for (auto& weak : m_Spawned)
	{
		if (auto spawn = weak.lock())
		{
			spawn->SetPendingKill(true);
		}
	}

	m_Spawned.clear();
	m_PendingKill = true;

	for (auto& circle : m_Circles)
	{
		if (circle)
		{
			COLLISION->RemoveCollider(circle.get());
		}
	}

	m_Circles.clear();
	m_CircleImg.clear();
}

void CStar::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);


}

void CStar::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);


}
