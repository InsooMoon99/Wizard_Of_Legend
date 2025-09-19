#include "stdafx.h"
#include "CGaia_Armor.h"

CGaia_Armor::CGaia_Armor(SkillInfo& _Info)
	: CSkill_Actor(_Info)
{
	m_Pos = _Info.SpawnPos;
	m_StartPos = _Info.SpawnPos;

}

CGaia_Armor::~CGaia_Armor()
{
}

void CGaia_Armor::BeginPlay()
{
	CSkill_Actor::BeginPlay();

	int frameNumber = 0;

	for (int i = 0; i < 8; i++)
	{
		m_Armor[i] = make_shared<ObCircle>();
		m_Armor[i]->scale = Vec2(100.f, 100.f);
		m_Armor[i]->SetParentRT(*m_Skill_Info.Instigator->Get_Collision());
		m_Armor[i]->isFilled = false;
		m_Armor[i]->visible = false;

		m_Angle[i] = i * (PI / 4);

		m_Armor[i]->SetLocalPos(Vec2(cosf(m_Angle[i]) * m_Radius, sinf(m_Angle[i]) * m_Radius));

		m_Armor_Img[i] = RESOURCE->CreateSprite(L"Skill\\GAIA_ARMOR.bmp", Vec2(600.f / 12.f) * 1.6f, Int2(12, 1), true);
		m_Armor_Img[i]->ChangeAnim(ANIMSTATE::LOOP, 0.2f, true);
		m_Armor_Img[i]->SetParentRT(*m_Armor[i]);
		m_Armor_Img[i]->frame.x = i;

	}

	
}

void CGaia_Armor::Tick()
{
	CSkill_Actor::Tick();

	if (m_PendingKill) return;

	// 위치 갱신
	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	for (int i = 0; i < 8; ++i)
	{
		m_Angle[i] -= 5.5f * DELTA;

		m_Armor[i]->SetLocalPos(Vec2(cosf(m_Angle[i]) * m_Radius,
			sinf(m_Angle[i]) * m_Radius));

	}

	for (auto& img : m_Armor_Img)
		img->Update();

	for (auto& armor : m_Armor)
		armor->Update();

}

void CGaia_Armor::LateUpdate()
{
	CSkill_Actor::LateUpdate();


}
	

void CGaia_Armor::Render()
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

void CGaia_Armor::Release()
{
	CSkill_Actor::Release();

	//if (m_Collision)
	//{
	//	COLLISION->RemoveCollider(m_Collision.get());
	//
	//	m_Collision.reset();
	//}
}

void CGaia_Armor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentBeginOverlap(collider, other);
}

void CGaia_Armor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CSkill_Actor::OnComponentEndOverlap(collider, other);
}
