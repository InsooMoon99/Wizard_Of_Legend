#include "stdafx.h"
#include "CMiddleBoss.h"
#include "CAttackActor.h"
#include "CPlayer.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"
#include "CUIComponent.h"
#include "CWeaponComponent.h"

CMiddleBoss::CMiddleBoss()
{
	m_Status->SetMaxHp(800.f);
	m_Status->SetHp(800.f);

	m_Weapon = CHelper::CreateSharedComponent<CWeaponComponent>(this);
	m_UI = CHelper::CreateSharedComponent<CUIComponent>(this);

	Resource_Settings();
}

CMiddleBoss::~CMiddleBoss()
{
	Release();

}

void CMiddleBoss::BeginPlay()
{
	CMonsterBase::BeginPlay();

	m_State->Change_State(ECharacterState::Middle_Spawn);

	if (m_Weapon)
	{
		m_Weapon->Equip_Skill(ESkillType::Middle_Fire);
		m_Weapon->Equip_Skill(ESkillType::Middle_Ball);
		m_Weapon->Equip_Skill(ESkillType::Middle_SpawnBall);
	}

	if (m_UI)
	{
		m_UI->SetMiddleHUD();
	}
}

void CMiddleBoss::Tick()
{
	CMonsterBase::Tick();


#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("Middle Hp : %f", m_Status->GetHp());
		ImGui::Text("Middle State : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
	}

	ImGui::End();
#endif

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	switch (m_State->Get_State())
	{
	case ECharacterState::Idle:		State_Idle();		 break;
	case ECharacterState::Walk:		State_Walk();		 break;
	case ECharacterState::Attack1:	State_Attack1();	 break;
	case ECharacterState::Attack2:	State_Attack2();	 break;
	case ECharacterState::Hit:		State_Hit();		 break;
	case ECharacterState::Dead:		State_Dead();		 break;
	case ECharacterState::Middle_Spawn:		State_Spawn();		 break;

	case ECharacterState::PS_END: break;
	}

	if (m_prevState != m_State->Get_State() || m_prevDir != m_State->Get_Dir())
		SetSprite(m_State->Get_State(), m_State->Get_Dir());


	if (INPUT->KeyDown('1'))
	{
		m_State->Change_State(ECharacterState::Attack3);
	}

	if (m_CurImg)
	{
		bool bCheck = (m_TraceDir.x > 0.f);
		m_CurImg->reverseLR = bCheck;

		if (bCheck)
		{
			m_Shadow->SetLocalPos(Vec2(0.5f, -80.f));
		}

		else
		{
			if (m_State->Get_State() != ECharacterState::Idle)
			{
				m_Shadow->SetLocalPos(Vec2(10.5f, -80.f));

			}
		}
		
	}
		

	// Color 변경
	Update_Hit();

	for (auto& col : m_vecCol)
		col->Update();

	if (m_CurImg)
		m_CurImg->Update();

	m_Shadow->Update();

	for (auto& component : m_Components)
		component->TickComponent();

	m_prevState = m_State->Get_State();
	m_prevDir = m_State->Get_Dir();

}

void CMiddleBoss::LateUpdate()
{
	CMonsterBase::LateUpdate();

	//if (m_Target && m_State->Get_State() == ECharacterState::Idle)
	//{
	//	m_State->Change_State(ECharacterState::Attack1);
	//}


}

void CMiddleBoss::Render()
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

void CMiddleBoss::Release()
{
	CMonsterBase::Release();

	if (m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		//m_Collision.reset();
	}

	m_vecCol.clear();
}

void CMiddleBoss::State_Spawn()
{
	if (m_CurImg == m_AppearImg)
	{
		if (m_CurImg->frame.x >= m_CurImg->maxFrame.x - 1)
		{
			m_AppearTime += DELTA;

			if (m_AppearTime >= 0.5f)
			{
				m_State->Change_State(ECharacterState::Idle);
			}
		}
	}

}

void CMiddleBoss::State_Idle()
{
	if (m_PendingA2)
	{
		m_PendingA2Timer += DELTA;
		if (m_PendingA2Timer >= m_PendingA2Delay)
		{
			m_PendingA2 = false;
			m_PendingA2Timer = 0.f;
			m_State->Change_State(ECharacterState::Attack2);
			return;
		}
		return;
	}

	m_IdleTimer += DELTA;

	if (m_IdleTimer >= m_IdleChangeTimer)
	{
		m_IdleTimer = 0.f;

		if (m_Target && m_Collision)
		{
			Vec2 to = m_Target->Get_WorldPos() - m_Collision->GetWorldPos();
			float distance = to.Length();

			if (distance <= 300.f)
				m_State->Change_State(ECharacterState::Attack1);
			else
				m_State->Change_State(ECharacterState::Walk);
		}
	}
}


void CMiddleBoss::State_Walk()
{
	// 플레이어 방향
	TraceTarget();

	MoveTile(m_TraceDir, 200.f);
}

void CMiddleBoss::State_Attack1()
{
	if (m_CurImg && m_CurImg->frame.x >= 3 && m_AttackCheck)
	{
		m_AttackCheck = false;
		m_Weapon->Activate_Skill(ESkillType::Middle_Fire);
		
	}

	if (m_CurImg && CHelper::IsLastFrame(m_CurImg))
	{
		m_PendingA2 = true;   
		m_PendingA2Timer = 0.f;   
		m_State->Change_State(ECharacterState::Idle);

		return;
	}

	SpriteResetIdle();
}

void CMiddleBoss::State_Attack2()
{
	if (m_CurImg && m_CurImg->frame.x >= 3 && m_AttackCheck)
	{
		m_AttackCheck = false;
		m_Weapon->Activate_Skill(ESkillType::Middle_Ball);
	}

	if (m_CurImg && CHelper::IsLastFrame(m_CurImg))
	{
		m_State->Change_State(ECharacterState::Idle);
		return;
	}
}


void CMiddleBoss::State_Hit()
{
	// 넉백
	if (m_Knock.active)
	{
		float t = m_Knock.t / max(m_Knock.duration, 1e-4f);
		t = clamp(t, 0.f, 1.f);

		float k = 1.f - t;
		float speed = m_Knock.amount * k;

		MoveTile(m_Knock.dir, speed);

		m_Knock.t += DELTA;
		if (m_Knock.t >= m_Knock.duration)
			m_Knock.active = false;
	}

	if (m_HitTimer <= 0.f)
		m_State->Change_State(ECharacterState::Idle);

	//SpriteResetIdle();
}

void CMiddleBoss::State_Dead()
{
	if (m_CurImg == m_Sprites[ECharacterState::Dead][EPlayerDir::Front])
	{
		if (CHelper::IsLastFrame(m_CurImg))
		{
			m_PendingKill = true;
		}
	}
	
}

void CMiddleBoss::Update_Hit()
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

void CMiddleBoss::Hit_Color()
{
	IColorChanger::Hit_Color();

	CHelper::HitColor(m_CurImg.get());
	m_HitTimer = 0.05f;
}

void CMiddleBoss::Hit_Effect()
{
	IColorChanger::Hit_Effect();


}

void CMiddleBoss::TraceTarget()
{
	CMonsterBase::TraceTarget();
	if (!m_Target || !m_Collision) return;

	ECharacterState cur = m_State->Get_State();
	if (cur == ECharacterState::Attack1 || cur == ECharacterState::Hit || cur == ECharacterState::Dead)
		return; 

	m_TraceDir = m_Target->Get_WorldPos() - m_Collision->GetWorldPos();
	float distance = m_TraceDir.Length();

	m_TraceDir.Normalize();

	if (distance <= 300.f) 
	{
		m_State->Change_State(ECharacterState::Attack1);
	}
			
	else 
	{
		if (cur != ECharacterState::Walk)
			m_State->Change_State(ECharacterState::Walk);
	}

}


void CMiddleBoss::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
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

void CMiddleBoss::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CMonsterBase::OnComponentEndOverlap(collider, other);


}

void CMiddleBoss::TakeDamage(FHitDamage _Hit)
{
	CMonsterBase::TakeDamage(_Hit);

	if (m_CurImg)
		CHelper::OrignColor(m_CurImg.get());

	//m_State->Change_State(ECharacterState::Hit);

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
		m_Knock.amount = _Hit.Amount * 0.2f;
	}

}

void CMiddleBoss::DropItem()
{
	//CMonsterBase::DropItem();

	//Vec2 dropPos = Vec2(1975.f, 2836.f);
	//
	//auto potal = make_shared<CPotal>();
	//potal->SetSpawnPos(dropPos);
	//
	//OBJECT->SpawnActor(potal);
}

void CMiddleBoss::SetSprite(ECharacterState _State, EPlayerDir _Dir)
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

void CMiddleBoss::Play_Sprite(ECharacterState _State)
{
	CMonsterBase::Play_Sprite(_State);

	CHelper::OrignColor(m_CurImg.get());
	m_AttackCheck = false;
	//m_IdleTimer = 0.f;

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		break;

	case ECharacterState::Walk:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case ECharacterState::Attack1:
		m_AttackCheck = true;
		SOUND->StopPlay("WAtk", 0.2f);
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		break;

	case ECharacterState::Attack2:
		m_AttackCheck = true;
		SOUND->StopPlay("WAtk", 0.2f);
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		break;

	case ECharacterState::Hit:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		CHelper::HitColor(m_CurImg.get()); 
		m_HitTimer = 0.15f;               
		break;

	case ECharacterState::Dead:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		SOUND->StopPlay("WDead", 0.2f);
		break;

	case ECharacterState::Middle_Spawn:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		break;
	}
}

void CMiddleBoss::Resource_Settings()
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
		Vec2 idleSize = Vec2(106.f, 145.f) * 1.3f;
		shared_ptr<ObImage> idle = RESOURCE->CreateImage(L"Monster\\Wizard\\Wizard_Idle.png", idleSize, true);
		idle->SetParentRT(*m_Collision);
		idle->SetLocalPosX(5.f);

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = idle;
	}

	// Walk
	{
		Vec2 walkSize = Vec2(428.f / 4.f, 145.f) * 1.3f;
		shared_ptr<ObImage> walk = RESOURCE->CreateSprite(L"Monster\\Wizard\\Wizard_Walk.png", walkSize, Int2(4, 1), true);
		walk->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Walk][EPlayerDir::Front] = walk;
	}

	// Attack1
	{
		Vec2 attackSize = Vec2(428.f / 4.f, 145.f) * 1.3f;
		shared_ptr<ObImage> attack = RESOURCE->CreateSprite(L"Monster\\Wizard\\Wizard_Attack.png", attackSize, Int2(4,1), true);
		attack->SetParentRT(*m_Collision);
		attack->SetLocalPosX(20.f);

		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front] = attack;
	}

	// Attack2
	{
		Vec2 attackSize = Vec2(428.f / 4.f, 145.f) * 1.3f;
		shared_ptr<ObImage> attack = RESOURCE->CreateSprite(L"Monster\\Wizard\\Wizard_Attack.png", attackSize, Int2(4, 1), true);
		attack->SetParentRT(*m_Collision);
		attack->SetLocalPosX(20.f);

		m_Sprites[ECharacterState::Attack2][EPlayerDir::Front] = attack;
	}

	// Attack3
	{
		Vec2 attackSize = Vec2(428.f / 4.f, 145.f) * 1.3f;
		shared_ptr<ObImage> attack = RESOURCE->CreateSprite(L"Monster\\Wizard\\Wizard_Attack.png", attackSize, Int2(4, 1), true);
		attack->SetParentRT(*m_Collision);
		attack->SetLocalPosX(20.f);

		m_Sprites[ECharacterState::Attack3][EPlayerDir::Front] = attack;
	}

	// hit
	{
		Vec2 hitSize = Vec2(214.f / 2.f, 145.f) * 1.3f;
		shared_ptr<ObImage> hit = RESOURCE->CreateSprite(L"Monster\\Wizard\\Wizard_Hit.png", hitSize, Int2(2, 1), true);
		hit->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Hit][EPlayerDir::Front] = hit;
	}

	// Dead
	{
		Vec2 deadSize = Vec2(428.f / 4.f, 146.f) * 1.3f;
		shared_ptr<ObImage> dead = RESOURCE->CreateSprite(L"Monster\\Wizard\\Wizard_Die.png", deadSize, Int2(4, 1), true);
		dead->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Dead][EPlayerDir::Front] = dead;
	}

	// Shadow
	{
		m_Shadow = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 3.65f);
		m_Shadow->SetParentRT(*m_Collision);
		m_Shadow->SetLocalPos(Vec2(15.5f, -80.f));
		m_Shadow->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		m_Shadow->visible = true;
	}

	// Spawn
	{
		m_AppearImg = RESOURCE->CreateSprite(L"Monster\\Wizard\\Appear_Wizard.png", Vec2(1080.f / 9.f, 187.f) * 1.2f, Int2(9,1), true);
		m_AppearImg->SetParentRT(*m_Collision);
		m_AppearImg->SetLocalPos(Vec2(8.f, 5.f));

		m_Sprites[ECharacterState::Middle_Spawn][EPlayerDir::Front] = m_AppearImg;
	}
	
	// Start Image Setting
	m_CurImg = m_Sprites[ECharacterState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}

void CMiddleBoss::MoveTile(const Vec2& dir, float speed)
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
