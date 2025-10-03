#include "stdafx.h"
#include "CSkill_Actor.h"

#include "CTile.h"

CSkill_Actor::CSkill_Actor(FSkillInfo& _Info)
	: m_Skill_Info(_Info)
{

	
}	

CSkill_Actor::~CSkill_Actor()
{
}

void CSkill_Actor::BeginPlay()
{
	CActor::BeginPlay();
}

void CSkill_Actor::Tick()
{
	CActor::Tick();
}

void CSkill_Actor::LateUpdate()
{
	CActor::LateUpdate();
}

void CSkill_Actor::Render()
{
	CActor::Render();
}

void CSkill_Actor::Release()
{
	CActor::Release();
}

void CSkill_Actor::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CActor::OnComponentBeginOverlap(collider, other);
}

void CSkill_Actor::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CActor::OnComponentEndOverlap(collider, other);
}

bool CSkill_Actor::CheckHitWall()
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

void CSkill_Actor::OnHitWall(const Vec2& hitPos)
{

}
