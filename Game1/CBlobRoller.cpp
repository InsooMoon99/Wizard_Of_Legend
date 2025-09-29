#include "stdafx.h"
#include "CBlobRoller.h"

#include "CPlayer.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"

CBlobRoller::CBlobRoller()
{
	m_Status->SetHp(100.f);

	m_RenderLayer = ERenderLayer::Render_GameObject;

	Resource_Settings();
}

CBlobRoller::~CBlobRoller()
{
	Release();

}

void CBlobRoller::BeginPlay()
{
	CMonsterBase::BeginPlay();

	m_State->Change_State(ECharacterState::Idle);
}

void CBlobRoller::Tick()
{
	CMonsterBase::Tick();


#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("BlobRoller Hp : %f", m_Status->GetHp());
		ImGui::Text("BlobState : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
	}
	ImGui::End();
#endif

	if (m_Collision)
	{
		m_Pos = m_Collision->GetWorldPos();
	}


	switch (m_State->Get_State())
	{
	case ECharacterState::Idle:		State_Idle();		 break;
	case ECharacterState::Attack1:	State_Attack1();	 break;
	case ECharacterState::Attack2:	State_Attack2();	 break;
	case ECharacterState::Hit:		State_Hit();		 break;
	case ECharacterState::Dead:		State_Dead();		 break;

	case ECharacterState::PS_END: break;
	}

	// 플레이어 방향
	TraceTarget();

	if (m_prevState != m_State->Get_State() || m_prevDir != m_State->Get_Dir())
		SetSprite(m_State->Get_State(), m_State->Get_Dir());

	// Color 변경
	Update_Hit();

	for (auto& col : m_vecCol)
		col->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& component : m_Components)
		component->TickComponent();

	m_prevState = m_State->Get_State();
	m_prevDir = m_State->Get_Dir();

}

void CBlobRoller::LateUpdate()
{
	CMonsterBase::LateUpdate();

	//if (m_Target && m_State->Get_State() == ECharacterState::Idle)
	//{
	//	m_State->Change_State(ECharacterState::Attack1);
	//}
}

void CBlobRoller::Render()
{
	CMonsterBase::Render();

	for (auto& col : m_vecCol)
		col->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& component : m_Components)
		component->Render();

}

void CBlobRoller::Release()
{
	CMonsterBase::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}
}

void CBlobRoller::State_Idle()
{
	m_IdleTimer += DELTA;

	//cout << "Idle : " << m_IdleTimer << endl;

	if (m_IdleTimer > 0.6f && m_Target)
	{
		m_State->Change_State(ECharacterState::Attack1);

		m_IdleTimer = 0.f;
	}
}

void CBlobRoller::State_Attack1()
{
	bool isLast = CHelper::IsLastFrame(m_CurImg);

	if (isLast && !m_OnLastFrameEdge)
	{
		m_OnLastFrameEdge = true;
		++m_LastFrameHitCount;

		if (m_LastFrameHitCount >= 2)
		{
			m_BehaviorCheck = false;
			m_State->Change_State(ECharacterState::Attack2);
			return;
		}
	}

	if (!isLast)
		m_OnLastFrameEdge = false;
}

void CBlobRoller::State_Attack2()
{
	m_Attack2Timer += DELTA;

	//cout << "Attack2 : " << m_Attack2Timer << endl;

	if (m_Attack2Timer < 4.f)
	{
		// TODO : 플레이어 추격
		m_Collision->MoveWorldPos(m_TraceDir * 400.f * DELTA);
	}

	else if (m_Attack2Timer > 4.f)
	{
		m_State->Change_State(ECharacterState::Idle);

		m_Attack2Timer = 0.f;
	}
}

void CBlobRoller::State_Hit()
{
	// 넉백
	if (m_Knock.active)
	{
		float time = m_Knock.t / max(m_Knock.duration, 1e-4f);
		time = clamp(time, 0.f, 1.f);

		float kTime = 1.f - time;
		float speed = m_Knock.amount * kTime;
		MoveTile(m_Knock.dir,speed);

		m_Knock.t += DELTA;
		if (m_Knock.t >= m_Knock.duration)
		{
			m_Knock.active = false;
		}
	}

	// 이미지
	if (m_CurImg)
	{
		if (!m_Knock.active && CHelper::IsLastFrame(m_CurImg))
		{
			CHelper::OrignColor(m_CurImg.get());
			m_HitTimer = 0.f; 

			m_State->Change_State(ECharacterState::Idle);
		}
	}
}

void CBlobRoller::State_Dead()
{
	if (m_CurImg = m_Sprites[ECharacterState::Dead][EPlayerDir::Front])
	{
		if (CHelper::IsLastFrame(m_CurImg))
		{
			// 소멸처리
			m_PendingKill = true;
		}
	}
	
}

void CBlobRoller::Update_Hit()
{
	IColorChanger::Update_Hit();

	if (m_HitTimer > 0.f)
	{
		m_HitTimer -= DELTA;

		if (m_HitTimer <= 0.f && m_CurImg)
		{
			CHelper::OrignColor(m_CurImg.get());
		}
	}
}

void CBlobRoller::Hit_Color()
{
	IColorChanger::Hit_Color();

	CHelper::HitColor(m_CurImg.get());
	m_HitTimer = 0.15f;
}

void CBlobRoller::Hit_Effect()
{
	IColorChanger::Hit_Effect();


}

void CBlobRoller::TraceTarget()
{
	CMonsterBase::TraceTarget();

	if (!m_Target) return;

	m_TraceDir = m_Target->Get_WorldPos() - m_Collision->GetWorldPos();
	float distance = m_TraceDir.Length();

	m_TraceDir.Normalize();

	if (m_TraceDir.x < 0.f)
	{
		m_CurImg->reverseLR = true;
	}

	else
	{
		m_CurImg->reverseLR = false;
	}

}

void CBlobRoller::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CMonsterBase::OnComponentBeginOverlap(collider, other);

	auto player = dynamic_cast<CPlayer*>(other->Get_Owner());
	if (!player) return;

	FHitDamage hitDamage;

	Vec2 dir = player->Get_WorldPos() - m_Collision->GetWorldPos();
	if (dir.LengthSquared() > 1e-8f) 
		dir.Normalize();

	hitDamage.Dir = dir;

	hitDamage.Damage = 10.f;
	hitDamage.colldier = collider;
	hitDamage.other = other;
	hitDamage.Amount = 700.f;
	hitDamage.Duration = 0.25;
	player->TakeDamage(hitDamage);

	m_IdleTimer = 0.f;
	m_State->Change_State(ECharacterState::Idle);

}

void CBlobRoller::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CMonsterBase::OnComponentEndOverlap(collider, other);


}

void CBlobRoller::TakeDamage(FHitDamage _Hit)
{
	CMonsterBase::TakeDamage(_Hit);

	if (m_CurImg) 
		CHelper::OrignColor(m_CurImg.get());

	m_State->Change_State(ECharacterState::Hit);
	m_Status->AddHp(-_Hit.Damage);

	if (m_Status->GetHp() <= 0.f)
	{
		SetDead();
		return;
	}

	if (_Hit.Duration > 1e-4f && _Hit.Amount > 0.f)
	{
		m_Knock.active = true;
		m_Knock.dir = _Hit.Dir;
		m_Knock.t = 0.f;
		m_Knock.duration = _Hit.Duration;
		m_Knock.amount = _Hit.Amount;
	}
}



void CBlobRoller::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	CMonsterBase::SetSprite(_State, _Dir);

	if (m_CurImg)
		m_CurImg->visible = false;

	auto& stateIter = m_Sprites[_State];
	auto imgIter = stateIter.find(_Dir);

	if (imgIter != stateIter.end())
	{
		m_CurImg = imgIter->second;
		m_CurImg->visible = true;

		Play_Sprite(_State);
	}
}

void CBlobRoller::Play_Sprite(ECharacterState _State)
{
	CMonsterBase::Play_Sprite(_State);

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		m_IdleTimer = 0.f;
		break;

	case ECharacterState::Attack1:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.15f);
		m_BehaviorCheck = true;
		m_LastFrameHitCount = 0;
		m_OnLastFrameEdge = false;
		break;

	case ECharacterState::Attack2:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.06f);
		m_BehaviorCheck = true;
		m_Attack2Timer = 0.f;
		break;

	case ECharacterState::Hit:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		CHelper::HitColor(m_CurImg.get());
		m_HitTimer = 0.15f;   
		break;

	case ECharacterState::Dead:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		break;
	}
}

void CBlobRoller::Resource_Settings()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(70.f, 70.f);
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	// Idle
	{
		Vec2 idleSize = Vec2(32.f, 32.f) * 3.5f;
		shared_ptr<ObImage> idle = RESOURCE->CreateImage(L"Monster\\BlobRoller\\Blolb_Idle2.png", idleSize, true);
		idle->SetParentRT(*m_Collision);
		idle->visible = true;

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = idle;
	}

	// WindUp
	{
		Vec2 hitSize = Vec2(64.f / 2.f, 32.f) * 3.5f;
		shared_ptr<ObImage> hit = RESOURCE->CreateSprite(L"Monster\\BlobRoller\\BlobRollerWindup.png", hitSize, Int2(2, 1), true);
		hit->SetParentRT(*m_Collision);
		hit->visible = true;

		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front] = hit;
	}

	// Attack
	{
		Vec2 attackSize = Vec2(224.f / 7.f, 32.f) * 3.5f;
		shared_ptr<ObImage> attack = RESOURCE->CreateSprite(L"Monster\\BlobRoller\\BlobRollerRoll.png", attackSize, Int2(7,1), true);
		attack->SetParentRT(*m_Collision);
		attack->visible = true;

		m_Sprites[ECharacterState::Attack2][EPlayerDir::Front] = attack;
	}

	// Hit
	{
		Vec2 hitSize = Vec2(96.f / 3.f, 32.f) * 3.5f;
		shared_ptr<ObImage> hit = RESOURCE->CreateSprite(L"Monster\\BlobRoller\\BlobRollerHurt.png", hitSize, Int2(3, 1), true);
		hit->SetParentRT(*m_Collision);
		hit->visible = true;

		m_Sprites[ECharacterState::Hit][EPlayerDir::Front] = hit;
	}

	// Dead
	{
		Vec2 deadSize = Vec2(160.f / 5.f, 32.f) * 3.5f;
		shared_ptr<ObImage> dead = RESOURCE->CreateSprite(L"Monster\\BlobRoller\\BlobRollerDead.png", deadSize, Int2(5, 1), true);
		dead->SetParentRT(*m_Collision);
		dead->visible = true;

		m_Sprites[ECharacterState::Dead][EPlayerDir::Front] = dead;
	}
	
	// Start Image Setting
	m_CurImg = m_Sprites[ECharacterState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}

void CBlobRoller::MoveTile(const Vec2& dir, float speed)
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
