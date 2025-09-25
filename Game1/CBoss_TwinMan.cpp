#include "stdafx.h"
#include "CBoss_TwinMan.h"

#include "CBoss_Controller.h"
#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "CStatusComponent.h"
#include "CUIComponent.h"

CBoss_TwinMan::CBoss_TwinMan(CBoss_Controller* _Controller)
	: CBossPawn(_Controller)
{
	if (m_Controller == nullptr)
		return;

	m_Weapon = CHelper::CreateSharedComponent<CWeaponComponent>(this);

	m_State->Change_State(m_Controller->GetState());

	Resource_Settings();
}

CBoss_TwinMan::~CBoss_TwinMan()
{
	Release();
}

void CBoss_TwinMan::BeginPlay()
{
	Super::BeginPlay();

	if (m_Controller == nullptr)
		return;

	auto controlState = GetControlState();
	if (controlState)
	{
		controlState->OnBossStateChanged.Add([this](ECharacterState _State, EPlayerDir _Dir)
			{
				this->OnStateChange(_State, _Dir);
			});
	}

	SetSprite(m_State->Get_State(), m_State->Get_Dir());

	// Skill
	m_Weapon->Equip_Skill(ESkillType::Boss_Man_Bat);
	m_Weapon->Equip_Skill(ESkillType::Boss_Man_Target);
	m_Weapon->Equip_Skill(ESkillType::Boss_Man_Star);
}

void CBoss_TwinMan::Tick()
{
	Super::Tick();

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	m_prevState = m_State->Get_State();
	m_prevDir = m_State->Get_Dir();


	switch (m_State->Get_State())
	{
	case ECharacterState::Boss_Start:
		State_Start();
		break;

	case ECharacterState::Boss_Start2:
		State_Start2();
		break;

	case ECharacterState::Boss_Ready:
		State_Ready();
		break;

	case ECharacterState::Idle:
		State_Idle();
		break;

	case ECharacterState::Walk:
		State_Walk();
		break;

	case ECharacterState::Boss_Attack1_Ready:
		State_Attack1_Ready();
		break;

	case ECharacterState::Boss_Attack1:
		State_Attack1();
		break;

	case ECharacterState::Boss_Attack2:
		State_Attack2();
		break;

	case ECharacterState::Boss_Attack3:
		State_Attack3();
		break;
	}

	if (INPUT->KeyDown('1'))
	{
		m_Controller->ChangeState(ECharacterState::Boss_Ready);
	}
	
	if (INPUT->KeyDown('2'))
	{
		m_Controller->ChangeState(ECharacterState::Boss_Attack2);
	}

	if (INPUT->KeyDown('3'))
	{
		m_Controller->ChangeState(ECharacterState::Boss_Attack3);
	}

	// Color 변경
	Update_Hit();

	// Update
	for (auto& col : m_vecCol)
		col->Update();

	m_FloorCrater->Update();

	if (m_CurImg)
		m_CurImg->Update();

	m_MetalOrb_Man->Update();

	for (auto& component : m_Components)
		component->TickComponent();
}

void CBoss_TwinMan::LateUpdate()
{
	Super::LateUpdate();

}

void CBoss_TwinMan::Render()
{
	Super::Render();

#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("Boss_Man : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
	}

	ImGui::End();

#endif

	for (auto& col : m_vecCol)
		col->Render();

	//PARTICLE->Render();

	m_FloorCrater->Render();

	if (m_CurImg)
		m_CurImg->Render();

	m_MetalOrb_Man->Render();
	
}

void CBoss_TwinMan::Release()
{
	Super::Release();

	m_Sprites.clear();

	m_CurImg.reset();

	for (auto& component : m_Components)
		SafeDelete(component);

	for (auto col : m_vecCol)
		SafeDelete(col);
}

void CBoss_TwinMan::Update_Hit()
{
	Super::Update_Hit();

	if (m_HitTimer > 0.f)
	{
		m_HitTimer -= DELTA;

		if (m_HitTimer <= 0.f)
		{
			CHelper::OrignColor(m_CurImg.get());
		}
	}
}

void CBoss_TwinMan::Hit_Color()
{
	Super::Hit_Color();

	CHelper::HitColor(m_CurImg.get());
	m_HitTimer = 0.15f;
}

void CBoss_TwinMan::Hit_Effect()
{
	Super::Hit_Effect();

}

void CBoss_TwinMan::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentBeginOverlap(collider, other);

	// Boss Hit
	//ReportHit(10.f);
	//cout << "Twin Man Hit" << endl;
}

void CBoss_TwinMan::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	Super::OnComponentEndOverlap(collider, other);

}

void CBoss_TwinMan::State_Start()
{
	// Boss_Start
	if (bAnimStarted && m_CurImg->frame.x >= m_CurImg->maxFrame.x - 1)
	{
		if (m_Controller)
		{
			SOUND->Play("ImpactLightning");
			SOUND->SetVolume("ImpactLightning", 0.15f);
			
			SOUND->Play("MetalBreak");
			SOUND->SetVolume("MetalBreak", 0.15f);

			m_Controller->OnBossStart.Broadcast();

			bAnimStarted = false;
		}
	}

}

void CBoss_TwinMan::State_Start2()
{
	// Boss_Start2
	if (bAnimStarted && m_CurImg->frame.x == m_CurImg->maxFrame.x - 6)
	{
		Vec2 pos = Vec2(m_Collision->GetWorldPos().x + m_Controller->GetOffSetPos().x, m_Collision->GetWorldPos().y + 50.f);

		PARTICLE->Spawn_Particle(L"Fist_All", pos);

		SOUND->Stop("ImpactLightning");
		SOUND->Stop("MetalBreak");

		SOUND->Play("ImpactLightning2");
		SOUND->SetVolume("ImpactLightning2", 0.15f);
	}

	if (bAnimStarted && m_CurImg->frame.x >= m_CurImg->maxFrame.x - 1)
	{
		bAnimStarted = false;
		m_Controller->ChangeState(ECharacterState::Boss_Ready);
	}
}

void CBoss_TwinMan::State_Ready()
{
	// Boss_Ready
	if (bAnimStarted && m_CurImg->frame.x >= m_CurImg->maxFrame.x - 1)
	{
		m_Controller->ChangeState(ECharacterState::Boss_Attack1_Ready);

		bAnimStarted = false;
	}
}

void CBoss_TwinMan::State_Idle()
{
	// TODO : 공격 대기 시간
}

void CBoss_TwinMan::State_Walk()
{
	// TODO : 플레이어 추격
}

void CBoss_TwinMan::State_Attack1_Ready()
{
	if (m_CurImg == m_Bat_Spawn)
	{
		if (m_CurImg->IsMaxframe())
		{
			if (m_Bat_Spawn)   m_Bat_Spawn->visible = false;
			if (m_Bat_Windup)  m_Bat_Windup->visible = true;

			m_CurImg = m_Bat_Windup;
			m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
			m_CurImg->frame = Int2(0, 0); 
		}
	}

	else if (m_CurImg == m_Bat_Windup)
	{
		if (m_CurImg->IsMaxframe())
		{
			if (m_Controller)
				m_Controller->ChangeState(ECharacterState::Boss_Attack1);
		}
	}
}

void CBoss_TwinMan::State_Attack1()
{
	if (m_CurImg == m_Sprites[ECharacterState::Boss_Attack1][EPlayerDir::Front])
	{
		if (m_CurImg->frame.x == 2 && !bCountBat)
		{
			m_AttackCount++;

			m_Weapon->Activate_Skill(ESkillType::Boss_Man_Bat);

			SOUND->Stop("MetalClang");
			SOUND->Play("MetalClang");
			SOUND->SetVolume("MetalClang", 0.2f);

			bCountBat = true;

			m_MetalOrb_Man->visible = true;
			m_MetalOrb_Man->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
			m_MetalOrb_Man->frame = Int2(0, 0);

			m_Controller->SetBallMovePos(Vec2(0, 0));
		}

		if (CHelper::IsLastFrame(m_CurImg))
		{
			bCountBat = false; 

			if (m_AttackCount >= 7)
			{
				m_CurImg = m_Bat_Throw;
				m_CurImg->visible = true;
				m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.15f);
				m_CurImg->frame = Int2(0, 0);

				m_MetalOrb_Man->visible = false;
			}
		}
	}

	else if (m_CurImg == m_Bat_Throw)
	{
		if (m_CurImg->IsMaxframe())
		{
			if (m_Controller)
			{
				m_Controller->ChangeState(ECharacterState::Idle);
			}
		}
	}
}

void CBoss_TwinMan::State_Attack2()
{
	if (m_CurImg == m_Sprites[ECharacterState::Boss_Attack2][EPlayerDir::Front])
	{
		if (CHelper::IsLastFrame(m_CurImg))
		{
			m_CurImg = m_Target_Throw;
			m_Target_Throw->visible = true;
			m_Target_Throw->ChangeAnim(ANIMSTATE::ONCE, 0.12f);
			m_Target_Throw->frame = Int2(0, 0);
		}
	}

	else if (m_CurImg == m_Target_Throw)
	{
		if (m_Target_Throw->frame.x >= 2 && bAttack2)
		{
			SOUND->StopPlay("MetalClang", 0.2f);

			m_Attack2Count++;
			m_Controller->OnThrowTargetOrb.Broadcast(m_Attack2Count - 1);
			bAttack2 = false;
		}

		if (CHelper::IsLastFrame(m_Target_Throw))
		{
			m_Target_Throw->ChangeAnim(ANIMSTATE::ONCE, 0.12f);
			m_Target_Throw->frame = Int2(0, 0);
			bAttack2 = true;
		}

		if (m_Attack2Count >= 5)
		{
			m_Target_Throw->ChangeAnim(ANIMSTATE::STOP, 0.f);

			OnStateChange(ECharacterState::Idle, EPlayerDir::Front);
			
		}
	}
}

void CBoss_TwinMan::State_Attack3()
{
	if (m_CurImg)
	{
		if (m_CurImg->frame.x == 1)
		{
			m_FloorCrater->visible = true;
		}

	}

}

void CBoss_TwinMan::OnStateChange(ECharacterState _State, EPlayerDir _Dir)
{
	CBossPawn::OnStateChange(_State, _Dir);

	m_State->Change_State(_State);
	SetSprite(_State, _Dir);
}

void CBoss_TwinMan::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	Super::SetSprite(_State, _Dir);

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

void CBoss_TwinMan::Play_Sprite(ECharacterState _State)
{
	Super::Play_Sprite(_State);

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.1f);
		break;

	case ECharacterState::Walk:
		//m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case ECharacterState::Attack1:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		//m_CurImg->frame = Int2(0, 0);
		bAttackCheck = true;
		break;

	case ECharacterState::Boss_Start:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		bAnimStarted = true;
		break;

	case ECharacterState::Boss_Start2:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		bAnimStarted = true;
		break;

	case ECharacterState::Boss_Ready:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		bAnimStarted = true;
		break;

	case ECharacterState::Boss_Attack1_Ready:
		//m_CurImg = m_Bat_Windup;
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		bAttackCheck = true;
		break;

	case ECharacterState::Boss_Attack1:
		m_AttackCount = 0;
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.08f); // 0.08
		break;

	case ECharacterState::Boss_Attack2:
		m_Weapon->Activate_Skill(ESkillType::Boss_Man_Target);

		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.25f);
		m_CurImg->frame = Int2(0, 0);
		bAttack2 = true;
		m_Attack2Count = 0;

		break;

	case ECharacterState::Boss_Attack3:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		break;


	}
}

void CBoss_TwinMan::Resource_Settings()
{
	Super::Resource_Settings();

	// Boss Collision
	{
		m_Collision = make_shared<ObRect>();
		m_Collision->scale = Vec2(80.f, 80.f);
		m_Collision->visible = true;
		m_Collision->isFilled = false;
		m_Collision->pivot = OFFSET_N;
		m_Collision->collider = COLLIDER::RECT;
		m_Collision->Set_Owner(this);
		m_Collision->SetWorldPos(Vec2(-1790.f, -590.f));
		COLLISION->AddCollider(m_Collision.get());

		m_vecCol.push_back(m_Collision.get());
	}
	
	// Start Setting
	{
		Vec2 start_Size = Vec2(225.f / 9.f, 45.f) * 3.f;
		shared_ptr<ObImage> start = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_ManStart.png", start_Size, Int2(9, 1), true);
		start->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Start][EPlayerDir::Front] = start;

		m_BossTaunt = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Taunt.png", Vec2(1200.f / 15.f, 60.f) * 3.f, Int2(15, 1), false);
		m_BossTaunt->SetParentRT(*m_Collision);
		m_BossTaunt->SetLocalPos(Vec2(20.f,0.f));

		m_Sprites[ECharacterState::Boss_Start2][EPlayerDir::Front] = m_BossTaunt;
	}

	// Idle
	{
		Vec2 start_Size = Vec2(24.f, 40.f) * 3.f;
		shared_ptr<ObImage> start = RESOURCE->CreateImage(L"Monster\\Lightning\\Boss_Idle.png", start_Size);
		start->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = start;
	}

	// Ready
	{
		Vec2 size = Vec2(720.f / 12.f, 105.f) * 3.f;
		shared_ptr<ObImage> ready = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Attack1_Ready.png", size, Int2(12,1));
		ready->SetParentRT(*m_Collision);
		ready->SetLocalPos(Vec2(7.f, 100.f));

		m_Sprites[ECharacterState::Boss_Ready][EPlayerDir::Front] = ready;
	}

	// Bat
	{
		Vec2 bat_Spawn_Size = Vec2(150.f / 3.f, 50.f) * 3.f;
		m_Bat_Spawn = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Bat_Spawn.png", bat_Spawn_Size, Int2(3, 1), true);
		m_Bat_Spawn->SetParentRT(*m_Collision);
		m_Bat_Spawn->SetLocalPosX(-15.f);
		m_Sprites[ECharacterState::Boss_Attack1_Ready][EPlayerDir::Front] = m_Bat_Spawn;

		Vec2 bat_Windup_Size = Vec2(150.f / 3.f, 50.f) * 3.6f;
		m_Bat_Windup = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Bat_Windup.png", bat_Windup_Size, Int2(3, 1), true);
		m_Bat_Windup->SetParentRT(*m_Collision);

		Vec2 bat_Throw_Size = Vec2(520.f / 8.f, 65.f) * 3.f;
		m_Bat_Throw = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Bat_Throw.png", bat_Throw_Size, Int2(8, 1), true);
		m_Bat_Throw->SetParentRT(*m_Collision);

		Vec2 bat_Idle_Size = Vec2(150.f / 3.f, 50.f) * 3.f;
		shared_ptr<ObImage> bat_Idle = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Bat_Idle.png", bat_Idle_Size, Int2(3, 1), true);
		bat_Idle->SetParentRT(*m_Collision);

		Vec2 bat_Swing_Size = Vec2(390.f / 6.f, 65.f) * 3.f;
		shared_ptr<ObImage> bat_Swing = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Bat_Swing.png", bat_Swing_Size, Int2(6, 1), true);
		bat_Swing->SetParentRT(*m_Collision);

		Vec2 metal_Orb_Size = Vec2(224.f / 7.f, 32.f) * 3.f;
		m_MetalOrb_Man = RESOURCE->CreateSprite(L"Monster\\Lightning\\MetalOrb_Man.png", metal_Orb_Size, Int2(7, 1), false);
		m_MetalOrb_Man->SetParentRT(*m_Collision);
		m_MetalOrb_Man->SetLocalPosY(-45.f);

		m_Sprites[ECharacterState::Boss_Attack1][EPlayerDir::Front] = bat_Swing;
	}

	// Target Orb
	{
		Vec2 spawn_Size = Vec2(325.f / 5.f, 65.f) * 3.f;
		m_Target_Spawn = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_OrbSummon.png", spawn_Size, Int2(5,1), true);
		m_Target_Spawn->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Attack2][EPlayerDir::Front] = m_Target_Spawn;

		Vec2 throw_Size = Vec2(400.f / 5.f, 60.f) * 3.f;
		m_Target_Throw = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Throw.png", spawn_Size, Int2(5, 1), true);
		m_Target_Throw->SetParentRT(*m_Collision);

		//m_Sprites[ECharacterState::Boss_Attack2][EPlayerDir::Front] = m_Target_Throw;
	}

	// Star
	{
		Vec2 star_Size = Vec2(195.f / 3.f, 65.f) * 3.f;
		shared_ptr<ObImage> ground = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_GroundSlam.png", star_Size, Int2(3, 1), true);
		ground->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Attack3][EPlayerDir::Front] = ground;

		m_FloorCrater = RESOURCE->CreateImage(L"Monster\\Lightning\\FloorCrater.png", Vec2(180.f, 180.f), false);
		m_FloorCrater->SetParentRT(*m_Collision);
		m_FloorCrater->SetLocalPosY(-50.f);
	}

	m_CurImg = m_Sprites[ECharacterState::Boss_Start][EPlayerDir::Front];

}