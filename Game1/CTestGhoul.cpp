#include "stdafx.h"
#include "CTestGhoul.h"

#include "CAttackActor.h"
#include "CMonster_Factory.h"
#include "CMonster_Melee.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"

CTestGhoul::CTestGhoul()
{
	m_Status->SetHp(2500.f);

	Resource_Settings();
}

CTestGhoul::~CTestGhoul()
{
	Release();

}

void CTestGhoul::BeginPlay()
{
	CMonsterBase::BeginPlay();

	m_State->Change_State(ECharacterState::Idle);
}

void CTestGhoul::Tick()
{
	CMonsterBase::Tick();


#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("Ghoul Hp : %f", m_Status->GetHp());
		ImGui::Text("Ghoul State : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
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
	case ECharacterState::Walk:		State_Walk();		 break;
	case ECharacterState::Attack1:	State_Attack1();	 break;
	case ECharacterState::Attack2:	State_Attack1();	 break;
	case ECharacterState::Dead:		State_Dead();		 break;

	case ECharacterState::PS_END: break;
	}

	if (m_prevState != m_State->Get_State() || m_prevDir != m_State->Get_Dir())
		SetSprite(m_State->Get_State(), m_State->Get_Dir());


	if (m_CurImg)
		m_CurImg->reverseLR = (m_TraceDir.x < 0.f);

	// Color 변경
	Update_Hit();

	for (auto& col : m_vecCol)
		col->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& component : m_Components)
		component->TickComponent();

	m_Shadow->Update();

	m_prevState = m_State->Get_State();
	m_prevDir = m_State->Get_Dir();

}

void CTestGhoul::LateUpdate()
{
	CMonsterBase::LateUpdate();

	//if (m_Target && m_State->Get_State() == ECharacterState::Idle)
	//{
	//	m_State->Change_State(ECharacterState::Attack1);
	//}

}

void CTestGhoul::Render()
{
	CMonsterBase::Render();

	for (auto& col : m_vecCol)
		col->Render();

	m_Shadow->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& component : m_Components)
		component->Render();

}

void CTestGhoul::Release()
{
	CMonsterBase::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();
	}

	m_vecCol.clear();
}

void CTestGhoul::State_Idle()
{
	m_IdleTimer += DELTA;

	if (m_IdleTimer >= 2.0f) 
	{
		m_IdleTimer = 0.f;

		if (m_Target && m_Collision)
		{
			Vec2 to = m_Target->Get_WorldPos() - m_Collision->GetWorldPos();
			float distance = to.Length();

			if (distance <= 120.f)
			{
				if (rand() % 2 == 0) 
					m_State->Change_State(ECharacterState::Attack1);

				else                 
					m_State->Change_State(ECharacterState::Attack2);
			}
			else
			{
				m_State->Change_State(ECharacterState::Walk);
			}
		}
	}
}

void CTestGhoul::State_Walk()
{
	// 플레이어 방향
	TraceTarget();

	MoveTile(m_TraceDir, 200.f);
}

void CTestGhoul::State_Attack1()
{
	if (!m_Action)
	{
		FMeleeParams params;
		params.moveStart	= 0.45f;
		params.moveEnd		= 0.75f;
		params.moveDist		= 160.f;
		params.hitTime		= 0.75f;
		params.duration		= 1.6f;
		params.damage		= 160.f;
		params.hitBoxSize	= Vec2(70.f, 120.f);
		params.AttackSound	= "Ghoul";

		m_Action = CMonster_Factory::CreateMelee(this, params);
		m_Action->OnStart(this); // OnStart의 생명주기는 몬스터쪽에서 담당
	}

	m_Action->OnUpdate(this, DELTA);

	if (m_Action->IsFinished())
	{
		m_Action->OnExit(this);
		m_Action.reset();
		m_State->Change_State(ECharacterState::Idle);
	}
}

void CTestGhoul::State_Attack2()
{
	if (!m_CurImg) return;

	if (m_CurImg->frame.x >= 4)
		MoveTile(m_TraceDir, 180.f);

	if (m_CurImg && m_CurImg->frame.x >= 3 && m_AttackCheck)
	{
		m_AttackCheck = false;

		FSkillInfo info{};
		info.Instigator = this;
		info.SpawnPos = m_Collision->GetWorldPos() + m_TraceDir * 100.f;

		SKILL->Spawn_SkillActor(ESkillActor::AttackCollision, info, Vec2(200.f));
	}

	if (m_CurImg->Get_AnimState() == ANIMSTATE::STOP)
	{
		m_State->Change_State(ECharacterState::Idle); 
	}
}

void CTestGhoul::State_Hit()
{
	
}

void CTestGhoul::State_Dead()
{
	if (m_CurImg == m_Sprites[ECharacterState::Dead][EPlayerDir::Front])
	{
		if (CHelper::IsLastFrame(m_CurImg))
		{
			m_PendingKill = true;
		}
	}
	
}

void CTestGhoul::Update_Hit()
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

void CTestGhoul::Hit_Color()
{
	IColorChanger::Hit_Color();

	CHelper::HitColor(m_CurImg.get());
	m_HitTimer = 0.05f;
}

void CTestGhoul::Hit_Effect()
{
	IColorChanger::Hit_Effect();


}

void CTestGhoul::TraceTarget()
{
	CMonsterBase::TraceTarget();
	if (!m_Target || !m_Collision) return;

	ECharacterState cur = m_State->Get_State();
	if (cur == ECharacterState::Attack1 || cur == ECharacterState::Attack2 ||
		cur == ECharacterState::Hit || cur == ECharacterState::Dead)
		return; 

	m_TraceDir = m_Target->Get_WorldPos() - m_Collision->GetWorldPos();
	float distance = m_TraceDir.Length();

	m_TraceDir.Normalize();

	if (distance <= 120.f) 
	{
		int random = rand() % 2;

		if (random == 0)
		{
			m_State->Change_State(ECharacterState::Attack1);
		}
		else 
		{
			m_State->Change_State(ECharacterState::Attack2);
		}
	}
			
	else 
	{
		if (cur != ECharacterState::Walk)
			m_State->Change_State(ECharacterState::Walk);
	}

}


void CTestGhoul::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CMonsterBase::OnComponentBeginOverlap(collider, other);

	//auto player = dynamic_cast<CPlayer*>(other->Get_Owner());
	//if (!player) return;
	//
	//FHitDamage hitDamage;
	//
	//Vec2 dir = player->Get_WorldPos() - m_Collision->GetWorldPos();
	//if (dir.LengthSquared() > 1e-8f) 
	//	dir.Normalize();
	//
	//hitDamage.Dir = dir;
	//
	//hitDamage.Damage = 10.f;
	//hitDamage.colldier = collider;
	//hitDamage.other = other;
	//hitDamage.Amount = 700.f;
	//hitDamage.Duration = 0.25;
	//player->TakeDamage(hitDamage);

}

void CTestGhoul::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CMonsterBase::OnComponentEndOverlap(collider, other);


}

void CTestGhoul::TakeDamage(FHitDamage _Hit)
{
	CMonsterBase::TakeDamage(_Hit);

	if (m_CurImg) 
		CHelper::OrignColor(m_CurImg.get());

	Hit_Color();

	if (m_Status->GetHp() <= 0.f)
	{
		SetDead();

		return;
	}

}

void CTestGhoul::SetSprite(ECharacterState _State, EPlayerDir _Dir)
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

void CTestGhoul::Play_Sprite(ECharacterState _State)
{
	CMonsterBase::Play_Sprite(_State);

	CHelper::OrignColor(m_CurImg.get());
	m_AttackCheck = false;
	m_IdleTimer = 0.f;

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		break;

	case ECharacterState::Walk:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.2f);
		break;

	case ECharacterState::Attack1:
		m_AttackCheck = true;
		//SOUND->StopPlay("Ghoul", 0.2f);
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.15f);

		break;

	case ECharacterState::Attack2:
		m_AttackCheck = true;
		//SOUND->StopPlay("Ghoul", 0.2f);
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.15f);

		break;

	case ECharacterState::Dead:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		break;
	}
}

void CTestGhoul::Resource_Settings()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(110.f, 140.f);
	m_Collision->visible = false;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	// Idle
	{
		Vec2 idleSize = Vec2(48.f, 64.f) * 3.5f;
		shared_ptr<ObImage> idle = RESOURCE->CreateImage(L"Monster\\GhoulLarge\\GhoulLargeIdle.png", idleSize, true);
		idle->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = idle;
	}

	// Walk
	{
		Vec2 walkSize = Vec2(240.f / 5.f, 64.f) * 3.5f;
		shared_ptr<ObImage> walk = RESOURCE->CreateSprite(L"Monster\\GhoulLarge\\GhoulLargeWalk.png", walkSize, Int2(5, 1), true);
		walk->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Walk][EPlayerDir::Front] = walk;
	}

	// Attack1
	{
		Vec2 attackSize = Vec2(280.f / 5.f, 64.f) * 3.5f;
		shared_ptr<ObImage> attack = RESOURCE->CreateSprite(L"Monster\\GhoulLarge\\GhoulLargeAttack0.png", attackSize, Int2(5,1), true);
		attack->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front] = attack;
	}

	// Attack2
	{
		Vec2 attackSize = Vec2(240.f / 5.f, 72.f) * 3.5f;
		shared_ptr<ObImage> attack = RESOURCE->CreateSprite(L"Monster\\GhoulLarge\\GhoulLargeAttack2.png", attackSize, Int2(5, 1), true);
		attack->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Attack2][EPlayerDir::Front] = attack;
	}

	// Dead
	{
		Vec2 deadSize = Vec2(288.f / 6.f, 64.f) * 3.5f;
		shared_ptr<ObImage> dead = RESOURCE->CreateSprite(L"Monster\\GhoulLarge\\GhoulLargeDead.png", deadSize, Int2(6, 1), true);
		dead->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Dead][EPlayerDir::Front] = dead;
	}

	m_Shadow = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 3.25f);
	m_Shadow->SetParentRT(*m_Collision);
	m_Shadow->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
	m_Shadow->SetLocalPos(Vec2(5.f, -80.f));
	m_Shadow->visible = true;

	
	// Start Image Setting
	m_CurImg = m_Sprites[ECharacterState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}

void CTestGhoul::MoveTile(const Vec2& dir, float speed)
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
}
