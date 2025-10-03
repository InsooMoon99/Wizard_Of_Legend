#include "stdafx.h"
#include "CCharacter.h"
#include "CStateComponent.h"
#include "CStatusComponent.h"

CCharacter::CCharacter()
{
	m_Status = CHelper::CreateSharedComponent<CStatusComponent>(this);
	m_State  = CHelper::CreateSharedComponent<CStateComponent>(this);

	m_prevState = ECharacterState::Idle;
	m_prevDir = EPlayerDir::Front;
}

CCharacter::~CCharacter()
{

}

void CCharacter::BeginPlay()
{
	CActor::BeginPlay();

}

void CCharacter::Tick()
{
	CActor::Tick();


}

void CCharacter::LateUpdate()
{
	CActor::LateUpdate();

}

void CCharacter::Render()
{
	CActor::Render();

}

void CCharacter::Release()
{
	CActor::Release();

}

void CCharacter::TakeDamage(FHitDamage _Hit)
{
	
}

void CCharacter::UpdateKnockbackTimer()
{
	if (!m_Knock.active) return;

	m_Knock.t += DELTA;

	if (m_Knock.t >= m_Knock.duration)
	{
		m_Knock.active = false;
	}
}

void CCharacter::SetDead()
{
	m_Status->SetHp(0.f);
	m_State->Change_State(ECharacterState::Dead);

	if(m_Collision) 
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision->collider = COLLIDER::NONE; 
		m_Collision->visible = false;           
	}
}

void CCharacter::MoveTile(const Vec2& dir, float speed)
{
	if (!m_Collision) return;

	ECharacterState playerState = m_State->Get_State();

	GameObject* col = m_Collision.get();

	Vec2 nDir = dir;
	if (nDir.LengthSquared() > 0.f)
		nDir.Normalize();

	const Vec2 nextMove = nDir * speed * DELTA;

	Vec2 half = col->scale * 0.5f;

	if (nextMove.x != 0.f)
	{
		Vec2 pos = col->GetWorldPos();
		Vec2 dirX = Vec2(nextMove.x, 0.f);
		Vec2 next = pos + dirX;

		Vec2 minW = next - half;
		Vec2 maxW = next + half;

		if (!TILE->IsBlocked_ExceptFall(minW, maxW, playerState == ECharacterState::Dash))
		{
			col->MoveWorldPos(dirX);
		}
		// else : 막히면 X축 이동 스킵
	}

	if (nextMove.y != 0.f)
	{
		Vec2 pos = col->GetWorldPos();
		Vec2 dirY = Vec2(0.f, nextMove.y);
		Vec2 next = pos + dirY;

		Vec2 minW = next - half;
		Vec2 maxW = next + half;

		if (!TILE->IsBlocked_ExceptFall(minW, maxW, playerState == ECharacterState::Dash))
		{
			col->MoveWorldPos(dirY);
		}
		// else : 막히면 Y축 이동 스킵
	}

	//Vec2 currentPos = col->GetWorldPos();
	//Vec2 currentMin = Vec2(currentPos.x - half.x - 5.0f, currentPos.y - half.y - 5.0f);
	//Vec2 currentMax = Vec2(currentPos.x + half.x - 5.0f, currentPos.y + half.y - 5.0f);
	//
	//vector<CTile*> overlappedTiles;
	//TILE->QueryTilesAABB(currentMin, currentMax, overlappedTiles);
	//
	//for (auto* tile : overlappedTiles)
	//{
	//	if (tile->Get_DrawID() == (int)ETileType::FALL)
	//	{
	//		if (playerState == EPlayerState::Idle || playerState == EPlayerState::Walk)
	//		{
	//			// TODO: 플레이어 낙하 로직 구현
	//			m_Collision->SetWorldPos(Vec2(0.f, 0.f));
	//		}
	//	}
	//}
}

