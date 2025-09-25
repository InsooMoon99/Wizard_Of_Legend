#include "stdafx.h"
#include "CBoss_TwinGirl.h"

#include "CBoss_Controller.h"
#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "CStatusComponent.h"
#include "CUIComponent.h"

CBoss_TwinGirl::CBoss_TwinGirl(CBoss_Controller* _Controller)
	: CBossPawn(_Controller)
{
	if (m_Controller == nullptr)
		return;

	m_Weapon = CHelper::CreateSharedComponent<CWeaponComponent>(this);

	m_State->Change_State(m_Controller->GetState());

	Resource_Settings();

}

CBoss_TwinGirl::~CBoss_TwinGirl()
{
	Release();
}

void CBoss_TwinGirl::BeginPlay()
{
	Super::BeginPlay();

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	// Delegate
	if (m_Controller)
	{
		auto controlState = GetControlState();
		if (controlState)
		{
			controlState->OnBossStateChanged.Add([this](ECharacterState _State, EPlayerDir _Dir)
				{
					this->OnStateChange(_State, _Dir);
				});
		}

		m_Controller->OnBossStart.Add([this]()
		{
				this->OnBossStart();
		});

		m_Controller->OnDashStart.Add([this](const Vec2& _TargetPos)
			{
				this->OnDashStart(_TargetPos);
			});

		m_Controller->OnBossStateChanged.Add([this](ECharacterState _State)
			{
				if (_State == ECharacterState::Boss_Start)
				{
					this->OnBossStart();
				}

			if (_State == ECharacterState::Boss_Attack1_Ready)
				this->OnBossReady();
			});

		m_Controller->OnJavelinStart.Add([this](const Vec2& _Pos)
			{
				this->OnJavelin(_Pos);
			});

		m_Controller->OnStarStart.Add([this](const Vec2& _Pos)
			{
				this->OnStarPos(_Pos);
			});
	}
	
	SetSprite(m_State->Get_State(), m_State->Get_Dir());

	// Skill
	m_Weapon->Equip_Skill(ESkillType::Boss_Girl_Dash);
	m_Weapon->Equip_Skill(ESkillType::Boss_Girl_Javelin);
	m_Weapon->Equip_Skill(ESkillType::Boss_Girl_Star);
}

void CBoss_TwinGirl::Tick()
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

	case ECharacterState::Air:
		State_Air();
		break;

	case ECharacterState::Walk:
		State_Walk();
		break;

	case ECharacterState::Boss_Attack1:
		State_Attack1();
		break;

	case ECharacterState::Boss_Attack2:
		State_Attack2();

	case ECharacterState::Boss_Attack3:
		State_Attack3();
		break;
	}

	// Color 변경
	Update_Hit();

	// Update
	for (auto& col : m_vecCol)
		col->Update();

	m_Teleport->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& component : m_Components)
		component->TickComponent();

}

void CBoss_TwinGirl::LateUpdate()
{
	Super::LateUpdate();

}

void CBoss_TwinGirl::Render()
{
	Super::Render();

#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("Boss_Girl : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
	}

	ImGui::End();

#endif

	for (auto& col : m_vecCol)
		col->Render();

	m_Teleport->Render();

	if (m_CurImg)
		m_CurImg->Render();
}

void CBoss_TwinGirl::Release()
{
	Super::Release();


	m_Sprites.clear();

	m_CurImg.reset();

	for (auto& component : m_Components)
		SafeDelete(component);

	for (auto col : m_vecCol)
		SafeDelete(col);
}

void CBoss_TwinGirl::Update_Hit()
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

void CBoss_TwinGirl::Hit_Color()
{
	Super::Hit_Color();

	CHelper::HitColor(m_CurImg.get());
	m_HitTimer = 0.15f;
}

void CBoss_TwinGirl::Hit_Effect()
{
	Super::Hit_Effect();

}

void CBoss_TwinGirl::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CBossPawn::OnComponentBeginOverlap(collider, other);

	// Boss Hit
	//ReportHit(10.f);
	//cout << "Twin Girl Hit" << endl;
}

void CBoss_TwinGirl::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CBossPawn::OnComponentEndOverlap(collider, other);

}

void CBoss_TwinGirl::OnStateChange(ECharacterState _State, EPlayerDir _Dir)
{
	CBossPawn::OnStateChange(_State, _Dir);

	m_State->Change_State(_State);
	SetSprite(_State, _Dir);
}

void CBoss_TwinGirl::State_Start()
{
	if (!m_CurImg) 
		return;
	
	if (m_CurImg->frame.x >= m_CurImg->maxFrame.x - 1)
	{
		m_Controller->OnBossStart.Broadcast();
	}

}

void CBoss_TwinGirl::State_Start2()
{
	
}

void CBoss_TwinGirl::State_Ready()
{
	

}

void CBoss_TwinGirl::State_Air()
{
	float shakeSpeed = 5.f;
	float shakeAmplitude = 10.f;

	m_ShakeTimer += DELTA * shakeSpeed;
	float shakeY = sin(m_ShakeTimer) * shakeAmplitude;

	Vec2 newPos = m_Collision->GetWorldPos();
	newPos.y = m_OriginalY + shakeY;
	m_Collision->SetWorldPos(Vec2(newPos.x, newPos.y + 370.f));

	m_waitTime -= DELTA;

	if (m_waitTime <= 0.f)
	{
		m_Controller->ChangeState(ECharacterState::Idle);
	}
}

void CBoss_TwinGirl::State_Idle()
{
	// TODO : 공격 대기 시간
	bFirst = true;
}

void CBoss_TwinGirl::State_Walk()
{
	// TODO : 플레이어 추격
}

void CBoss_TwinGirl::State_Attack1()
{
	if (m_CurImg == m_Teleport_Floor)
	{
		if (CHelper::IsLastFrame(m_CurImg))
		{
			m_CurImg->visible = false;

			//m_CurImg = m_Dash_Left;
		}
	}

}

void CBoss_TwinGirl::State_Attack2()
{
	// 점프일 때 움직이게
	if (m_CurImg == m_Jump)
	{
		if (!m_CurImg->IsMaxframe() && bFirst)
		{
			m_Collision->MoveWorldPos(UP * 250.f * DELTA);
		}

		else
		{
			float shakeSpeed = 5.f; 
			float shakeAmplitude = 10.f; 

			m_ShakeTimer += DELTA * shakeSpeed;
			float shakeY = sin(m_ShakeTimer) * shakeAmplitude;

			Vec2 newPos = m_Collision->GetWorldPos();
			newPos.y = m_OriginalY + shakeY;
			m_Collision->SetWorldPos(Vec2(newPos.x, newPos.y + 370.f));

			if (m_JavelinPos.x != 0.f)
			{
				m_CurImg = m_JavelinSpawn;
				m_JavelinSpawn->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
				m_JavelinSpawn->frame = Int2(0, 0);

				bJavelinCheck = true;
			}
		}
	}
	// Spawn
	else if (m_CurImg == m_JavelinSpawn)
	{
		if (m_CurImg->IsMaxframe())
		{
			m_CurImg = m_JavelinThrow;
			m_JavelinThrow->ChangeAnim(ANIMSTATE::LOOP, 0.115f);
			m_JavelinThrow->frame = Int2(0, 0);
		}
	}
	// Throw
	else if (m_CurImg == m_JavelinThrow)
	{
		m_CurImg->visible = true;

		Vec2 target = m_JavelinPos + Vec2(0.f, 240.f);

		Vec2 toTarget = target - m_Collision->GetWorldPos();
		float dist = toTarget.Length();
		float speed = 1600.f;
		float step = speed * DELTA;

		if (dist <= step)
			m_Collision->SetWorldPos(target);

		else
		{
			toTarget /= dist;
			m_Collision->MoveWorldPos(toTarget * step);
		}

		if (m_CurImg->frame.x == 1 && bJavelinCheck == true && m_JavelinCount < 5)
		{
			m_JavelinCount++;
			m_Weapon->Activate_Skill(ESkillType::Boss_Girl_Javelin);
			bJavelinCheck = false;
		}

		if (CHelper::IsLastFrame(m_CurImg))
		{
			bJavelinCheck = true;
		}

		if (m_JavelinCount == 5)
		{
			OnStateChange(ECharacterState::Air, EPlayerDir::Front);

			return;
		}
	}
}

void CBoss_TwinGirl::State_Attack3()
{
	if (m_CurImg == m_Jump)
	{
		if (m_CurImg->IsMaxframe())
		{
			if (m_UpTimer > 0.f)
			{
				m_UpTimer -= DELTA;

				m_Collision->MoveWorldPos(UP * 1250.f * DELTA);
			}

		}
	}

	else if (m_CurImg == m_StarStart)
	{
		//cout << "GOGO" << endl;
	}

}

void CBoss_TwinGirl::OnBossStart()
{
	if (!bStartPlay)
	{
		if (m_CurImg)
		{
			m_CurImg->visible = true;
			m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		}
		bStartPlay = true; 
	}
}

void CBoss_TwinGirl::OnBossReady()
{
	if (!m_Teleport) return;

	// 텔포
	m_CurImg = m_Teleport;

	if (m_CurImg == m_Teleport)
	{
		m_CurImg->visible = true;
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.2f);
		m_CurImg->frame = Int2(0, 0);
	}
	
}

void CBoss_TwinGirl::OnDashStart(const Vec2& _TargetPos)
{
	m_Controller->SetBallMovePos(_TargetPos);

	m_DashDir = _TargetPos - m_Pos;
	m_DashDir.Normalize();

	if (m_DashDir.x <= 0)
		m_CurImg = m_Dash_Left;
	else
		m_CurImg = m_Dash_Right;

	m_CurImg->visible = true;
	m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
	m_CurImg->frame = Int2(0, 0);

	m_Weapon->Activate_Skill(ESkillType::Boss_Girl_Dash);
}

void CBoss_TwinGirl::OnJavelin(const Vec2& _Pos)
{
	m_JavelinPos = _Pos;

}

void CBoss_TwinGirl::OnStarPos(const Vec2& _Pos)
{
	// 도착 위치 설정
	m_StarPos = _Pos;
	m_Collision->SetWorldPos(m_StarPos);

	m_StarStart->visible = true;
	m_StarStart->ChangeAnim(ANIMSTATE::ONCE, 0.15f);
	m_CurImg = m_StarStart;

	SOUND->StopPlay("ImpactLightning2", 0.15f);
	PARTICLE->Spawn_Particle(L"Particle_Land", Vec2(_Pos.x, _Pos.y + 90.f), Vec2(1.5f));
}

void CBoss_TwinGirl::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	CBossPawn::SetSprite(_State, _Dir);

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

void CBoss_TwinGirl::Play_Sprite(ECharacterState _State)
{
	CBossPawn::Play_Sprite(_State);

	switch (_State)
	{
	case ECharacterState::Idle:
		m_JavelinPos = Vec2(0.f);
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		break;

	case ECharacterState::Walk:
		//m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		break;

	case ECharacterState::Air:
		m_waitTime = 1.f;
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.f);
		break;

	case ECharacterState::Boss_Start:
		m_CurImg->visible = false;
		break;

	case ECharacterState::Boss_Start2:
		m_CurImg->visible = false;
		break;

	case ECharacterState::Boss_Ready:
		m_CurImg->visible = false;
		//m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		{
			Vec2 pos = Vec2(m_Controller->GetManPos().x, m_Controller->GetManPos().y + 100.f);
			m_Collision->SetWorldPos(pos);
		}
		break;

	case ECharacterState::Boss_Attack1:
		m_JavelinPos = Vec2(0.f);
		m_Collision->SetWorldPos(Vec2(m_Controller->GetManPos().x + 50.f, m_Controller->GetManPos().y));
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		bDashCheck = true;

		break;

	case ECharacterState::Boss_Attack2:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.3f);
		m_JavelinCount = 0;
		{
			Vec2 pos = Vec2(m_Controller->GetManPos().x, m_Controller->GetManPos().y + 20.f);
			m_Collision->SetWorldPos(pos);

			PARTICLE->Spawn_Particle(L"Light_Land", Vec2(pos.x, pos.y + 120.f), Vec2(1.5f));
		}
		bFirst = true;
		m_JavelinPos = Vec2(0.f);
		
		break;

	case ECharacterState::Boss_Attack3:
		m_UpTimer = 0.3f;
		m_StarPos = Vec2(0.f); // 스킬 시작 전 초기화

		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.2f);
		m_Weapon->Activate_Skill(ESkillType::Boss_Girl_Star);

		break;

	}
}

void CBoss_TwinGirl::Resource_Settings()
{
	CBossPawn::Resource_Settings();

	// Boss Collision
	{
		m_Collision = make_shared<ObRect>();
		m_Collision->scale = Vec2(80.f, 80.f);
		m_Collision->visible = true;
		m_Collision->isFilled = false;
		m_Collision->pivot = OFFSET_N;
		m_Collision->collider = COLLIDER::RECT;
		m_Collision->Set_Owner(this);
		m_Collision->SetWorldPos(Vec2(-1590.f, -590.f));
		COLLISION->AddCollider(m_Collision.get());

		m_vecCol.push_back(m_Collision.get());
	}

	// Start Setting
	{
		Vec2 start_Idle_Size = Vec2(500.f / 10.f, 100.f) * 3.f;
		shared_ptr<ObImage> start_Idle = RESOURCE->CreateSprite(L"Monster\\Lightning\\GirlLand_Full.png", start_Idle_Size, Int2(10,1), false);
		start_Idle->SetParentRT(*m_Collision);
		start_Idle->SetLocalPos(Vec2(2.f,78.f));
		start_Idle->visible = false;
		//start_Idle->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Sprites[ECharacterState::Boss_Start][EPlayerDir::Front] = start_Idle;

		m_BossTaunt = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Taunt.png", Vec2(1200.f / 15.f, 60.f) * 3.f, Int2(15, 1), false);
		m_BossTaunt->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Start2][EPlayerDir::Front] = m_BossTaunt;
	}

	// Idle
	{
		Vec2 start_Size = Vec2(24.f, 40.f) * 3.f;
		shared_ptr<ObImage> start = RESOURCE->CreateImage(L"Monster\\Lightning\\Boss_GirlHandHip.png", start_Size);
		start->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = start;
	}

	// Ready : 필요없을듯 ? Man을 Main으로 하면
	{
		Vec2 size = Vec2(840.f / 12.f, 110.f) * 3.f;
		shared_ptr<ObImage> ready = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Ready.png", size, Int2(12, 1));
		ready->SetParentRT(*m_Collision);
		
		m_Sprites[ECharacterState::Boss_Ready][EPlayerDir::Front] = ready;
	}

	// Teleport
	{
		Vec2 size = Vec2(150.f / 5.f, 60.f) * 3.f;
		m_Teleport = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_Teleport_Long.png", size, Int2(5, 1));
		m_Teleport->SetParentRT(*m_Collision);
		m_Teleport->SetLocalPosY(40.f);

		Vec2 size2 = Vec2(210.f / 6.f, 55.f) * 3.f;
		m_Teleport_Floor = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_TeleportFloor.png", size2, Int2(6, 1));
		m_Teleport_Floor->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Attack1][EPlayerDir::Front] = m_Teleport_Floor;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = m_Teleport_Floor;
		//m_Sprites[ECharacterState::Boss_Attack1][EPlayerDir::Front] = m_Teleport;
	}

	// Dash
	{
		Vec2 size = Vec2(240.f / 6.f, 50.f) * 3.f;
		m_Dash_Left = RESOURCE->CreateSprite(L"Monster\\Lightning\\Girl_LeftDash.png", size, Int2(6,1));
		m_Dash_Left->SetParentRT(*m_Collision);

		m_Dash_Right = RESOURCE->CreateSprite(L"Monster\\Lightning\\Girl_RightDash.png", size, Int2(6, 1));
		m_Dash_Right->SetParentRT(*m_Collision);
	}

	// Javelin
	{
		Vec2 jump_Size = Vec2(160.f / 2.f, 80.f) * 3.f;
		m_Jump = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Girl_Jump.png", jump_Size, Int2(2, 1));
		m_Jump->SetParentRT(*m_Collision);

		m_Sprites[ECharacterState::Boss_Attack2][EPlayerDir::Front] = m_Jump;

		Vec2 javelin_SpawnSize = Vec2(240.f / 3.f, 80.f) * 3.f;
		m_JavelinSpawn = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Girl_JavelinSpawn.png", javelin_SpawnSize, Int2(3, 1), true);
		m_JavelinSpawn->SetParentRT(*m_Collision);

		Vec2 javelin_ThrowSize = Vec2(320.f / 4.f, 80.f) * 3.f;
		m_JavelinThrow = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Girl_JavelinThrow.png", javelin_ThrowSize, Int2(4, 1), true);
		m_JavelinThrow->SetParentRT(*m_Collision);

		shared_ptr<ObImage> JumpOnly = RESOURCE->CreateSprite(L"Monster\\Lightning\\Boss_Girl_Jump.png", jump_Size, Int2(2, 1), true);
		JumpOnly->SetParentRT(*m_Collision);
		JumpOnly->frame.x = 1;

		m_Sprites[ECharacterState::Air][EPlayerDir::Front] = JumpOnly;
	}

	// Star
	{
		m_Sprites[ECharacterState::Boss_Attack3][EPlayerDir::Front] = m_Jump;

		Vec2 startSize = Vec2(736.f / 23.f, 48.f) * 3.f;
		m_StarStart = RESOURCE->CreateSprite(L"Monster\\Lightning\\LightningGirlPBAoE_LOOP_premultiplied.png", startSize, Int2(23,1), true);
		m_StarStart->SetParentRT(*m_Collision);
		m_StarStart->SetLocalPosY(20.f);
		//m_StarStart->color = Color(0.5f, 0.5f, 0.5f, 0.5f);
	}

	m_Pos = m_Collision->GetWorldPos();

}
