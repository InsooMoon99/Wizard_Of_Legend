#include "stdafx.h"
#include "CPlayer.h"
#include "CCharacter.h"
#include "CResourceManager.h"
#include "CInventoryComponent.h"
#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "CUIComponent.h"
#include "CStatusComponent.h"
#include "CCollisionManager.h"
#include "CItem.h"
#include "CTile.h"

CPlayer::CPlayer()
{
	m_Weapon = CHelper::CreateSharedComponent<CWeaponComponent>(this);
	m_UI = CHelper::CreateSharedComponent<CUIComponent>(this);
	m_Inven = CHelper::CreateSharedComponent<CInventoryComponent>(this);

	m_Status->SetMaxHp(500.f);
	m_Status->SetMaxMp(100.f);

	m_Status->SetHp(380.f);
	m_Status->SetMp(40.f);    
	m_Status->SetSpeed(400.f);

	m_Status->SetGold(500);

	m_Status->SetAttack(20);
	m_Status->SetDef(15);

	m_TeamID = 0;

	// 컴포넌트 세팅하고 호출해야함
	Resource_Settings();

	if (m_UI)
	{
		//m_UI->SetPlayerHUD();
		//m_UI->SetInventoryHUD();
	}

	m_RenderLayer = ERenderLayer::RenderPlayer;
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::BeginPlay()
{
	CActor::BeginPlay();

	if (m_Weapon)
	{
		// Fire
		m_Weapon->Equip_Skill(ESkillType::Fire_Ball, ESkillSlot::Q);
		m_Weapon->Equip_Skill(ESkillType::Fire_Dragon, ESkillSlot::RButton);
		m_Weapon->Equip_Skill(ESkillType::Fire_Meteor, ESkillSlot::R);

		// Wind
		m_Weapon->Equip_Skill(ESkillType::Wind_Slash, ESkillSlot::LButton);
		m_Weapon->Equip_Skill(ESkillType::Wind_Dash, ESkillSlot::Space);

		// Earth
		m_Weapon->Equip_Skill(ESkillType::Earth_Shield, ESkillSlot::E);
	}

}

void CPlayer::Tick()
{
	CActor::Tick();

	Print_ImGui();

	float currentMp = m_Status->GetMp();
	float maxMp = m_Status->GetMaxMp();

	currentMp += 0.1f;

	float clampedMp = min(currentMp, maxMp);

	m_Status->SetMp(clampedMp);

	if (m_Collision)
		m_Pos = m_Collision->GetWorldPos();

	ECharacterState prevState = m_prevState;
	EPlayerDir      prevDir = m_prevDir;

	Key_Input();

	if (m_State->Get_State() == ECharacterState::Idle || m_State->Get_State() == ECharacterState::Walk)
	{
		if (INPUT->KeyDown(VK_SPACE))
		{
			m_State->Change_State(ECharacterState::Dash);
		}
		else if (m_MoveDir.x != 0 || m_MoveDir.y != 0)
		{
			m_State->Change_State(ECharacterState::Walk);

			m_LastDir = m_MoveDir;
			m_LastDir.Normalize();
		}
		else
		{
			m_State->Change_State(ECharacterState::Idle);
		}
	}

	switch (m_State->Get_State())
	{
	case ECharacterState::Idle: State_Idle(); break;
	case ECharacterState::Walk: State_Walk(); break;
	case ECharacterState::Dash: State_Dash(); break;
	case ECharacterState::Attack1:
	case ECharacterState::Attack2: State_Attack(); break;
	case ECharacterState::Attack3: State_Attack3(); break;
	case ECharacterState::Hit: State_Hit(); break;
	}

	UpdateKnockbackTimer();

	ECharacterState curState = m_State->Get_State();
	if (prevState != curState)
	{
		SetSprite(curState, m_State->Get_Dir()); 
	}
	else if (prevDir != m_State->Get_Dir())
	{
		if (curState == ECharacterState::Attack1 || curState == ECharacterState::Attack2
			|| curState == ECharacterState::Attack3)
			SwapKeepFrame(curState, m_State->Get_Dir()); 
		else
			SetSprite(curState, m_State->Get_Dir());
	}

	Update_Hit();

	
	for (auto& col : m_vecCol)     
		col->Update();

	for (auto& shadow : m_Shadow) 
		shadow.second->Update();

	m_Player_Marker->Update();
	m_Player_HitBlur->Update();

	if (m_CurImg)
		m_CurImg->Update();

	for (auto& dash : m_Dash)
		dash.second->Update();

	for (auto& component : m_Components)
		component->TickComponent();

	m_prevState = m_State->Get_State();
	m_prevDir = m_State->Get_Dir();

}


void CPlayer::LateUpdate()
{
	CActor::LateUpdate();

	 
}

void CPlayer::Render()
{
	CActor::Render();

	for (auto& col : m_vecCol)
		col->Render();

	for (auto& shadow : m_Shadow)
		shadow.second->Render();

	m_Player_Marker->Render();

	if (m_CurImg)
		m_CurImg->Render();

	for (auto& dash : m_Dash)
		dash.second->Render();

	for (auto& component : m_Components)
		component->Render();

	m_Player_HitBlur->Render();
}

void CPlayer::Release()
{
	CActor::Release();

	m_Sprites.clear();
	m_Shadow.clear();
	m_Dash.clear();

	m_CurImg.reset();

	for (auto& component : m_Components)
		SafeDelete(component);

	for (auto col : m_vecCol)
		SafeDelete(col);

	SOUND->DeleteSound("PlayerStep");
}

void CPlayer::Update_Hit()
{
	CCharacter::Update_Hit();

	if (!m_Player_HitBlur) return;

	// Hit 상태 의향이 있거나, 넉백 중이면 켠다
	bool wantOn = m_HitBlurWantsOn || m_Knock.active;

	if (wantOn)
		m_HitBlurAlpha += (m_HitBlurFadeIn > 0.f ? DELTA / m_HitBlurFadeIn : 1.f);
	else
		m_HitBlurAlpha -= (m_HitBlurFadeOut > 0.f ? DELTA / m_HitBlurFadeOut : 1.f);

	m_HitBlurAlpha = clamp(m_HitBlurAlpha, 0.f, 1.f);

	float alpha = m_HitBlurMaxAlpha * m_HitBlurAlpha;
	m_Player_HitBlur->color.A(alpha);
	m_Player_HitBlur->visible = (alpha > 0.01f);
}

void CPlayer::Hit_Color()
{
	CCharacter::Hit_Color();

}

void CPlayer::Hit_Effect()
{
	CCharacter::Hit_Effect();

}

void CPlayer::Key_Input()
{
	if (m_State->Get_State() == ECharacterState::Dash ||
		m_State->Get_State() == ECharacterState::Hit)
	{
		return;
	}

	if (INPUT->KeyDown(VK_TAB))
	{
		m_UI->ToggleInventory();
	}

	if (m_UI->GetToggle())
	{
		m_MoveDir = Vec2(0.f, 0.f);
		return;
	}
	
	if (m_State->GetCanAttack())
	{
		m_SkillMove = false;

		if (INPUT->KeyDown('Q'))
		{
			m_Weapon->Activate_Skill(ESkillType::Fire_Ball);
		}
		if (INPUT->KeyDown(VK_LBUTTON))
		{
			m_SkillMove = true;
			m_Weapon->Activate_Skill(ESkillType::Wind_Slash);
		}
		if (INPUT->KeyDown('E'))
		{
			m_Weapon->Activate_Skill(ESkillType::Earth_Shield);
		}
		if (INPUT->KeyDown(VK_RBUTTON))
		{
			m_Weapon->Activate_Skill(ESkillType::Fire_Dragon);
		}
		if (INPUT->KeyDown('R'))
		{
			m_Weapon->Activate_Skill(ESkillType::Fire_Meteor);
		}
	}

	Vec2 dirInput(0.f, 0.f);

	if (INPUT->KeyPress('W')) dirInput.y += 1.f;
	if (INPUT->KeyPress('S')) dirInput.y -= 1.f;
	if (INPUT->KeyPress('A')) dirInput.x -= 1.f;
	if (INPUT->KeyPress('D')) dirInput.x += 1.f;

	m_MoveDir = dirInput;

	if (m_MoveDir.Length() > 0.f && m_State->Get_State() == ECharacterState::Walk)
	{
		// 방향값이 있으면, WalkTimer 증가
		m_WalkTimer += DELTA;

		if (m_WalkTimer > 1.3f && m_Status->GetSpeed() < 600.f)
		{
			SOUND->StopPlay("RunSpeed", 0.05f);
			PARTICLE->Spawn_ParticleDir(L"DashEffect", m_Collision->GetWorldPos(), Vec2(-1), m_MoveDir);
			m_Status->SetSpeed(600.f);

			m_WalkTimer = 0.f;
		}
	}

	else if (m_MoveDir.Length() <= 0.f)
	{
		ResetSpeed();
	}

	if (m_State->Get_State() == ECharacterState::Walk) 
	{
		if (dirInput.x != 0.f || dirInput.y != 0.f) 
		{
			SOUND->Play("PlayerStep");
			SOUND->SetVolume("PlayerStep", 0.2f);
		}
	}

	// 대각 우선
	EPlayerDir curtDir = m_State->Get_Dir();
	if (dirInput.x != 0.f && dirInput.y != 0.f)
	{
		if (dirInput.x > 0.f && dirInput.y > 0.f)
		{
			curtDir = EPlayerDir::Back_Right;
			m_Player_Marker->rotation = -(PI / 4.f);
		}
		else if (dirInput.x < 0.f && dirInput.y > 0.f)
		{
			curtDir = EPlayerDir::Back_Left;
			m_Player_Marker->rotation = +(PI / 4.f);
		}
		else if (dirInput.x < 0.f && dirInput.y < 0.f)
		{
			curtDir = EPlayerDir::Front_Left;
			m_Player_Marker->rotation = -PI - (PI / 4.f);
		}
		else if (dirInput.x > 0.f && dirInput.y < 0.f)
		{
			curtDir = EPlayerDir::Front_Right;
			m_Player_Marker->rotation = -PI + (PI / 4.f);
		}
	}
	else
	{
		if (dirInput.y > 0.f)
		{
			curtDir = EPlayerDir::Back;
			m_Player_Marker->rotation = 2 * PI;
		}
		else if (dirInput.y < 0.f)
		{
			curtDir = EPlayerDir::Front;
			m_Player_Marker->rotation = -PI;
		}
		else if (dirInput.x < 0.f)
		{
			curtDir = EPlayerDir::Left;
			m_Player_Marker->rotation = +(PI / 2);
		}
		else if (dirInput.x > 0.f)
		{
			curtDir = EPlayerDir::Right;
			m_Player_Marker->rotation = -(PI / 2);
		}
		// 입력이 없으면 기존 방향 유지
	}

	m_State->Change_Dir(curtDir);

	UseItem();
}


void CPlayer::State_Idle()
{
	// TODO : Idle 상태를 따로 처리해야하는가?
	//m_PlayerCol->MoveWorldPos(m_MoveDir * m_Speed * DELTA);
}

void CPlayer::State_Walk()
{
	// 방향이 있으면 ㄱㄱ
	if (m_MoveDir.Length() > 0.f)
	{
		MoveTile(m_MoveDir, m_Status->GetSpeed());
	}
}

void CPlayer::State_Dash()
{
	ResetSpeed();

	if (m_CurImg->frame.x >= m_DashEndFrame)
		m_State->Change_State(ECharacterState::Idle);

	else
	{
		MoveTile(m_LastDir, m_DashDistance);

		// 그림자 크기 조절
		{
			if (m_CurImg->frame.x < 5)
			{
				float lerpTime = (float)m_CurImg->frame.x / 5.0f;
				m_Shadow[ECharacterState::Dash]->scale = Vec2::Lerp(m_StartShadowScale, m_EndShadowScale, lerpTime);
			}
			else if (m_CurImg->frame.x < m_DashEndFrame)
			{
				float lerpTime = (float)(m_CurImg->frame.x - 5) / (float)(m_DashEndFrame - 5);
				m_Shadow[ECharacterState::Dash]->scale = Vec2::Lerp(m_EndShadowScale, m_StartShadowScale, lerpTime);
			}
		}

	}
}

void CPlayer::State_Attack()
{
	if (m_MoveDir.Length() > 0.f && m_SkillMove)
	{
		float move = CHelper::AttackMoveFactorByFrame(m_CurImg.get());

		MoveTile(m_MoveDir, m_Status->GetSpeed() * move);
	}

	auto& map = m_Sprites[m_State->Get_State()];
	auto it = map.find(m_State->Get_Dir());
	if (it != map.end() && m_CurImg != it->second)
		return;

	if (m_CurImg->frame.x >= m_CurImg->maxFrame.x - 6)
		m_State->SetCanAttack(true);

	if (m_CurImg && m_CurImg->frame.x >= m_CurImg->maxFrame.x - 1)
	{
		if (m_Weapon && m_Weapon->IsFiringSkill())
		{
			m_State->Change_State(m_State->Set_AttackType());

			SetSprite(m_State->Get_State(), m_State->Get_Dir());
			return;
		}

		m_State->Change_State(ECharacterState::Idle);
	}
}

void CPlayer::State_Attack3()
{
	auto& map = m_Sprites[m_State->Get_State()];
	auto it = map.find(m_State->Get_Dir());
	if (it != map.end() && m_CurImg != it->second)
		return;

	if (m_CurImg->frame.x >= m_CurImg->maxFrame.x - 6)
		m_State->SetCanAttack(true);

	if (m_CurImg  && CHelper::IsLastFrame(m_CurImg))
	{
		m_State->Change_State(ECharacterState::Idle);
	}
}

void CPlayer::State_Hit()
{
	if (m_CurImg)
	{
		if (!m_Knock.active && CHelper::IsLastFrame(m_CurImg))
		{
			m_State->Change_State(ECharacterState::Idle);
			m_HitBlurWantsOn = false;
		}

		// 넉백
		if (m_Knock.active)
		{
			float time = m_Knock.t / max(m_Knock.duration, 1e-4f);
			time = clamp(time, 0.f, 1.f);

			float kTime = 1.f - time;
			float speed = m_Knock.amount * kTime;

			MoveTile(m_Knock.dir, speed);

			m_Knock.t += DELTA;
			if (m_Knock.t >= m_Knock.duration)
			{
				m_Knock.active = false;
			}
		}

	}
}


void CPlayer::SetSprite(ECharacterState _State, EPlayerDir _Dir)
{
	CCharacter::SetSprite(_State, _Dir);

	if (m_CurImg)
		m_CurImg->visible = false;

	for (auto& shadow : m_Shadow)
		shadow.second->visible = false;

	for (auto& dash : m_Dash)
		dash.second->visible = false;

	auto& stateIter = m_Sprites[_State];
	auto imgIter = stateIter.find(_Dir);

	if (imgIter != stateIter.end())
	{
		m_CurImg = imgIter->second;
		m_CurImg->visible = true;

		Play_Sprite(_State);
	}

	Shadow_Size();
}

void CPlayer::Play_Sprite(ECharacterState _State)
{
	bool bBlurCheck = _State == ECharacterState::Hit;
	m_HitBlurWantsOn = bBlurCheck;

	if (!bBlurCheck) m_State->SetCanAttack(true);

	if (_State != ECharacterState::Walk)
	{
		m_WalkTimer = 0.f;
		ResetSpeed();
	}
	

	switch (_State)
	{
	case ECharacterState::Idle:
		m_CurImg->ChangeAnim(ANIMSTATE::STOP, 0.0f);
		break;

	case ECharacterState::Walk:
		m_CurImg->ChangeAnim(ANIMSTATE::LOOP, 0.08f);
		break;

	case ECharacterState::Hit:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		m_CurImg->frame = Int2(0, 0);
		break;

	case ECharacterState::Dash:
		SOUND->Stop("PlayerStep");
		 
		SOUND->Stop("Dash");
		SOUND->Play("Dash");
		SOUND->SetVolume("Dash", 0.1f);

		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.03f);
		m_CurImg->frame = Int2(0, 0);
		m_DashEndFrame = m_CurImg->maxFrame.x - 1;

		Dash_Sprite();
		break;

	case ECharacterState::Attack1:
		m_State->SetCanAttack(false);
		if (m_Weapon->IsFiringSkill())
		{
			m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.048f);
			m_CurImg->frame = Int2(0, 0);
		}

		else
		{
			m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.06f);
			m_CurImg->frame = Int2(0, 0);
		}

		break;

	case ECharacterState::Attack2:
		m_State->SetCanAttack(false);
		if (m_Weapon->IsFiringSkill())
		{
			m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.048f);
			m_CurImg->frame = Int2(0, 0);
		}

		else
		{
			m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.06f);
			m_CurImg->frame = Int2(0, 0);
		}

		break;

	case ECharacterState::Attack3:
		m_CurImg->ChangeAnim(ANIMSTATE::ONCE, 0.08f);
		m_CurImg->frame = Int2(0, 0);

		break;
	}
}

void CPlayer::TakeDamage(FHitDamage _Hit)
{
	CCharacter::TakeDamage(_Hit);

	//if (m_State->Get_State() != ECharacterState::Hit)
	//	return;

	SOUND->StopPlay("PlayerHit", 0.5f);
	CAM->StartShake(5.f, 8.f, 0.15f);

	m_State->Change_State(ECharacterState::Hit);
	m_HitBlurWantsOn = true;

	float damage = CHelper::CaculateDamage(_Hit.Damage, m_Status->GetDef());
	CHelper::ApplyDamage(m_Status, damage, m_Collision->GetWorldPos());

	if (_Hit.Duration > 1e-4f && _Hit.Amount > 0.f)
	{
		m_Knock.active = true;
		m_Knock.dir = _Hit.Dir;                 
		m_Knock.t = 0.f;
		m_Knock.duration = _Hit.Duration;            
		m_Knock.amount = _Hit.Amount;              
	}

	//SetSprite(ECharacterState::Hit, m_State->Get_Dir());

	// Particle 
	Vec2 randomVec = Vec2(rand() % 20, rand() % 40);
	Vec2 particlePos = (_Hit.colldier->GetWorldPos() + _Hit.other->GetWorldPos()) / 2.0f;
	PARTICLE->Spawn_ParticleDir(L"HitSpark_Red", particlePos + randomVec);
	
}

void CPlayer::Attack_Sprite()
{

}

void CPlayer::Shadow_Size()
{
	// Shadow
	if (m_Shadow.find(m_State->Get_State()) != m_Shadow.end())
	{
		if (m_State->Get_State() == ECharacterState::Dash)
		{
			m_Shadow[ECharacterState::Dash]->scale = m_StartShadowScale;
		}

		m_Shadow[m_State->Get_State()]->visible = true;

	}
}

void CPlayer::Dash_Sprite()
{
	// 대쉬 이펙트
	auto iter = m_Dash.find(m_State->Get_Dir());
	if (iter != m_Dash.end())
	{
		shared_ptr<ObImage> dashEffect = iter->second;
		dashEffect->SetWorldPos(m_Collision->GetWorldPos());
		dashEffect->visible = true;
		dashEffect->ChangeAnim(ANIMSTATE::ONCE, 0.03f);
		dashEffect->frame = Int2(0, 0);
	}

}

void CPlayer::AddInteract(shared_ptr<CItem> _InItem)
{
	m_Interact.push_back(_InItem);
}

void CPlayer::RemoveInteract(CItem* _InItem)
{
	m_Interact.erase(remove_if(m_Interact.begin(), m_Interact.end(),
		[_InItem](const weak_ptr<CItem>& weak)
			{
				auto ptr = weak.lock();
				return !ptr || ptr.get() == _InItem;

			}), m_Interact.end());
}

void CPlayer::UseItem()
{
	float best = FLT_MAX;
	shared_ptr<CItem> bestPtr;

	for (auto it = m_Interact.begin(); it != m_Interact.end();)
	{
		if (auto s = it->lock())
		{
			float d2 = (s->Get_WorldPos() - m_Collision->GetWorldPos()).LengthSquared();
			if (d2 < best)
			{
				best = d2; bestPtr = s;
			}
			++it;
		}
		else
		{
			it = m_Interact.erase(it);
		}
	}
	m_CurItem = bestPtr;

	if (INPUT->KeyDown('F'))
	{
		if (auto cur = m_CurItem.lock())
		{
			cur->ActivateItem(this);
		}
	}
}

void CPlayer::Resource_Settings()
{
	// Player Collision
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(60.f, 60.f);
	m_Collision->SetLocalPosY(-30.f);
	m_Collision->visible = false;
	m_Collision->isFilled = false;
	m_Collision->pivot = OFFSET_N;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	m_vecCol.push_back(m_Collision.get());

	// State_Idle
	{
		Vec2 idle_Size = Vec2(90.f, 93.f) * 1.85f;
		shared_ptr<ObImage> front_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), true);
		front_Idle->SetParentRT(*m_Collision);
		front_Idle->SetLocalPos(Vec2(-8.f, 10.f));

		shared_ptr<ObImage> front_Left_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), true);
		front_Left_Idle->SetParentRT(*m_Collision);
		front_Left_Idle->SetLocalPos(Vec2(-8.f, 10.f));

		shared_ptr<ObImage> front_Right_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), true);
		front_Right_Idle->SetParentRT(*m_Collision);
		front_Right_Idle->SetLocalPos(Vec2(-10.f, 10.f));

		shared_ptr<ObImage> back_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), false);
		back_Idle->SetParentRT(*m_Collision);
		back_Idle->SetLocalPos(Vec2(5.f, 10.f));
		back_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> back_Left_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), false);
		back_Left_Idle->SetParentRT(*m_Collision);
		back_Left_Idle->SetLocalPos(Vec2(6.f, 10.f));
		back_Left_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> back_Right_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), false);
		back_Right_Idle->SetParentRT(*m_Collision);
		back_Right_Idle->SetLocalPos(Vec2(4.f, 10.f));
		back_Right_Idle->frame = Int2(1, 1);

		shared_ptr<ObImage> left_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), false);
		left_Idle->SetParentRT(*m_Collision);
		left_Idle->SetLocalPos(Vec2(-6.f, 10.f));
		left_Idle->frame = Int2(2, 1);

		shared_ptr<ObImage> right_Idle = RESOURCE->CreateSprite(L"Player\\Idle\\Idle.png", idle_Size, Int2(4, 1), false);
		right_Idle->SetParentRT(*m_Collision);
		right_Idle->SetLocalPos(Vec2(-27.f, 10.f));
		right_Idle->frame = Int2(3, 1);

		m_Sprites[ECharacterState::Idle][EPlayerDir::Front] = front_Idle;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Front_Left] = front_Left_Idle;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Front_Right] = front_Right_Idle;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Back] = back_Idle;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Back_Left] = back_Left_Idle;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Back_Right] = back_Right_Idle;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Left] = left_Idle;
		m_Sprites[ECharacterState::Idle][EPlayerDir::Right] = right_Idle;
	}

	// State_Walk
	{
		Vec2 walkSize = Vec2(900.f / 10.f, 93.f) * 1.7f;

		shared_ptr<ObImage> front_Walk = RESOURCE->CreateSprite(L"Player\\Front\\Front_Walk.png", walkSize, Int2(10, 1), false);
		front_Walk->SetParentRT(*m_Collision);
		front_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);

		m_Sprites[ECharacterState::Walk][EPlayerDir::Front] = front_Walk;
		m_Sprites[ECharacterState::Walk][EPlayerDir::Front_Left] = front_Walk;
		m_Sprites[ECharacterState::Walk][EPlayerDir::Front_Right] = front_Walk;

		shared_ptr<ObImage> back_Walk = RESOURCE->CreateSprite(L"Player\\Back\\Back_Walk.png", walkSize, Int2(10, 1), false);
		back_Walk->SetParentRT(*m_Collision);
		back_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);

		m_Sprites[ECharacterState::Walk][EPlayerDir::Back] = back_Walk;
		m_Sprites[ECharacterState::Walk][EPlayerDir::Back_Left] = back_Walk;
		m_Sprites[ECharacterState::Walk][EPlayerDir::Back_Right] = back_Walk;

		shared_ptr<ObImage> left_Walk = RESOURCE->CreateSprite(L"Player\\Left\\Left_Walk.png", walkSize, Int2(10, 1), false);
		left_Walk->SetParentRT(*m_Collision);
		left_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[ECharacterState::Walk][EPlayerDir::Left] = left_Walk;

		shared_ptr<ObImage> right_Walk = RESOURCE->CreateSprite(L"Player\\Right\\Right_Walk.png", walkSize, Int2(10, 1), false);
		right_Walk->SetParentRT(*m_Collision);
		right_Walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		m_Sprites[ECharacterState::Walk][EPlayerDir::Right] = right_Walk;
	}

	// Shadow
	{
		shared_ptr<ObImage> shadow_Idle = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.45f);
		shadow_Idle->SetParentRT(*m_Collision);
		shadow_Idle->SetLocalPos(Vec2(0.5f, -50.f));
		shadow_Idle->color = Color(0.0f, 0.0f, 0.0f, 0.15f);
		shadow_Idle->visible = false;
		m_Shadow.insert({ ECharacterState::Idle, shadow_Idle });

		shared_ptr<ObImage> shadow_Attack = RESOURCE->CreateImage(L"Shadow\\MIDDLE_SHADOW.bmp", Vec2(70.f, 70.f) * 1.5f);
		shadow_Attack->SetParentRT(*m_Collision);
		shadow_Attack->SetLocalPos(Vec2(0.0f, -50.f));
		shadow_Attack->color = Color(0.0f, 0.0f, 0.0f, 0.1f);
		m_Shadow.insert({ ECharacterState::Attack1, shadow_Attack });
		m_Shadow.insert({ ECharacterState::Attack2, shadow_Attack });
		m_Shadow.insert({ ECharacterState::Attack3, shadow_Attack });

		m_Shadow.insert({ ECharacterState::Hit, shadow_Idle });

		shared_ptr<ObImage> shadow_Walk = RESOURCE->CreateImage(L"Shadow\\MIDDLE_SHADOW.bmp", Vec2(60.f, 60.f) * 1.5f);
		shadow_Walk->SetParentRT(*m_Collision);
		shadow_Walk->SetLocalPos(Vec2(-.5f, -50.f));
		shadow_Walk->color = Color(0.0f, 0.0f, 0.0f, 0.25f);
		shadow_Walk->visible = false;
		m_Shadow.insert({ ECharacterState::Walk, shadow_Walk });

		m_StartShadowScale = Vec2(70.f, 70.f) * 1.5f;
		m_EndShadowScale = Vec2(80.f, 70.f) * 1.6f;

		shared_ptr<ObImage> shadow_Dash = RESOURCE->CreateImage(L"Shadow\\NORMAL_SHADOW.bmp", Vec2(60.f, 60.f) * 1.2f, false);
		shadow_Dash->SetParentRT(*m_Collision);
		shadow_Dash->SetLocalPos(Vec2(-2.5f, -50.f));
		shadow_Dash->color = Color(0.0f, 0.0f, 0.0f, 0.25f);

		m_Shadow.insert({ ECharacterState::Dash, shadow_Dash });
	}

	// State_Dash
	{
		Vec2 dash_Size = Vec2(1100.f / 11, 104.f) * 1.7f;

		shared_ptr<ObImage> front_Dash = RESOURCE->CreateSprite(L"Player\\front\\Front_Dash_long.png", dash_Size, Int2(11, 1), false);
		front_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> front_Left_Dash = RESOURCE->CreateSprite(L"Player\\front\\Front_Dash_long.png", dash_Size, Int2(11, 1), false);
		front_Left_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> front_Right_Dash = RESOURCE->CreateSprite(L"Player\\front\\Front_Dash_long.png", dash_Size, Int2(11, 1), false);
		front_Right_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> back_Dash = RESOURCE->CreateSprite(L"Player\\Back\\Back_Dash_long.png", dash_Size, Int2(11, 1), false);
		back_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> back_Left_Dash = RESOURCE->CreateSprite(L"Player\\Back\\Back_Dash_long.png", dash_Size, Int2(11, 1), false);
		back_Left_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> back_Right_Dash = RESOURCE->CreateSprite(L"Player\\Back\\Back_Dash_long.png", dash_Size, Int2(11, 1), false);
		back_Right_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> left_Dash = RESOURCE->CreateSprite(L"Player\\Left\\Left_Dash_long.png", dash_Size, Int2(11, 1), false);
		left_Dash->SetParentRT(*m_Collision);

		shared_ptr<ObImage> right_Dash = RESOURCE->CreateSprite(L"Player\\Right\\Right_Dash_long.png", dash_Size, Int2(11, 1), false);
		right_Dash->SetParentRT(*m_Collision);
		right_Dash->SetLocalPosX(-10.f);

		m_Sprites[ECharacterState::Dash][EPlayerDir::Front] = front_Dash;
		m_Sprites[ECharacterState::Dash][EPlayerDir::Front_Left] = front_Left_Dash;
		m_Sprites[ECharacterState::Dash][EPlayerDir::Front_Right] = front_Right_Dash;

		m_Sprites[ECharacterState::Dash][EPlayerDir::Back] = back_Dash;
		m_Sprites[ECharacterState::Dash][EPlayerDir::Back_Left] = back_Left_Dash;
		m_Sprites[ECharacterState::Dash][EPlayerDir::Back_Right] = back_Right_Dash;

		m_Sprites[ECharacterState::Dash][EPlayerDir::Left] = left_Dash;
		m_Sprites[ECharacterState::Dash][EPlayerDir::Right] = right_Dash;

	}

	// Dash_Effect
	{
		Vec2 dash_Effect_Size = Vec2(220.f / 11.f, 60.f) * 3.5f;
		float dash_Alpha = 0.3f;

		shared_ptr<ObImage> dash_Front_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", dash_Effect_Size, Int2(11, 1), false);
		dash_Front_Effect->rotation = -(PI / 2.f);
		dash_Front_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Front, dash_Front_Effect });

		// 좌하단
		shared_ptr<ObImage> dash_Front_Left_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", dash_Effect_Size, Int2(11, 1), false);
		dash_Front_Left_Effect->rotation = -3.f * PI / 4.f;
		dash_Front_Left_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Front_Left, dash_Front_Left_Effect });

		// 우하단
		shared_ptr<ObImage> dash_Front_Right_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", dash_Effect_Size, Int2(11, 1), false);
		dash_Front_Right_Effect->rotation = -(PI / 4.f);
		dash_Front_Right_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Front_Right, dash_Front_Right_Effect });

		shared_ptr<ObImage> dash_Back_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", dash_Effect_Size, Int2(11, 1), false);
		dash_Back_Effect->rotation = PI / 2.f;
		dash_Back_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Back, dash_Back_Effect });

		// 좌상단
		shared_ptr<ObImage> dash_Back_Left_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", dash_Effect_Size, Int2(11, 1), false);
		dash_Back_Left_Effect->rotation = 3.f * PI / 4.f;
		dash_Back_Left_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Back_Left, dash_Back_Left_Effect });

		// 우상단
		shared_ptr<ObImage> dash_Back_Right_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", dash_Effect_Size, Int2(11, 1), false);
		dash_Back_Right_Effect->rotation = PI / 4.f;
		dash_Back_Right_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Back_Right, dash_Back_Right_Effect });

		shared_ptr<ObImage> dash_Right_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", Vec2(220.f / 11.f, 60.f) * 3.5f, Int2(11, 1), false);
		dash_Right_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Right, dash_Right_Effect });

		shared_ptr<ObImage> dash_Left_Effect = RESOURCE->CreateSprite(L"Effect\\Dash_Effect.png", Vec2(220.f / 11.f, 60.f) * 3.5f, Int2(11, 1), false);
		dash_Left_Effect->color.A(dash_Alpha);
		m_Dash.insert({ EPlayerDir::Left, dash_Left_Effect });
	}

	// Attack
	{
		// Front
		Vec2 attack_Size{ (720.f / 8.f, 93.f) * 1.7f };

		shared_ptr<ObImage> attack_Front1 = RESOURCE->CreateSprite(L"Player\\Front\\Front_Left_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Front1->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front] = attack_Front1;
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front_Left] = attack_Front1;
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Front_Right] = attack_Front1;

		shared_ptr<ObImage> attack_Front2 = RESOURCE->CreateSprite(L"Player\\Front\\Front_Left_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Front2->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Front] = attack_Front2;
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Front_Left] = attack_Front2;
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Front_Right] = attack_Front2;

		// Back
		shared_ptr<ObImage> attack_Back1 = RESOURCE->CreateSprite(L"Player\\Back\\Back_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Back1->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Back] = attack_Back1;

		shared_ptr<ObImage> attack_Back2 = RESOURCE->CreateSprite(L"Player\\Back\\Back_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Back2->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Back] = attack_Back2;
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Back_Left] = attack_Back2;
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Back_Right] = attack_Back2;

		// Left
		shared_ptr<ObImage> attack_Left1 = RESOURCE->CreateSprite(L"Player\\Left\\Left_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Left1->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Left] = attack_Left1;
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Back_Left] = attack_Left1;

		shared_ptr<ObImage> attack_Left2 = RESOURCE->CreateSprite(L"Player\\Left\\Left_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Left2->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Left] = attack_Left2;
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Back_Left] = attack_Left2;

		// Right
		auto attack_Right1 = RESOURCE->CreateSprite(L"Player\\Right\\Right_Attack1.png", attack_Size, Int2(8, 1), false);
		attack_Right1->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Right] = attack_Right1;
		m_Sprites[ECharacterState::Attack1][EPlayerDir::Back_Right] = attack_Right1;

		auto attack_Right2 = RESOURCE->CreateSprite(L"Player\\Right\\Right_Attack2.png", attack_Size, Int2(8, 1), false);
		attack_Right2->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Right] = attack_Right2;
		m_Sprites[ECharacterState::Attack2][EPlayerDir::Back_Right] = attack_Right2;


	}

	{
		Vec2 attack3_Size = Vec2 (1000.f / 10.f, 112.f) * 1.5f;

		shared_ptr<ObImage> attack3 = RESOURCE->CreateSprite(L"Player\\Player_Attack3_3.png", attack3_Size, Int2(10, 1), false);
		attack3->SetParentRT(*m_Collision);
		attack3->SetLocalPos(Vec2(-10.f, 17.f));
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Front] = attack3;
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Front_Left] = attack3;
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Front_Right] = attack3;
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Left] = attack3;
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Right] = attack3;

		shared_ptr<ObImage> attack3_back = RESOURCE->CreateSprite(L"Player\\Player_Attack3_Back2.png", attack3_Size, Int2(10, 1), false);
		attack3_back->SetParentRT(*m_Collision);
		attack3_back->SetLocalPos(Vec2(+7.f, 22.f));
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Back] = attack3_back;
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Back_Left] = attack3_back;
		m_Sprites[ECharacterState::Attack3][EPlayerDir::Back_Right] = attack3_back;
	}

	// Direction Overlay
	{
		// TODO : Glory 도 추가하기?
		m_Player_Marker = RESOURCE->CreateImage(L"UI\\Player_Marker.png", Vec2(32.f, 33.f) * 3.f);
		m_Player_Marker->SetParentRT(*m_Collision);
		m_Player_Marker->SetLocalPos(Vec2(-2.f, -35.f));
		m_Player_Marker->rotation = -PI;
		m_Player_Marker->color.A(0.15f);
		m_Player_Marker->visible = true;
	}

	// Hit
	{
		Vec2 hitSize = Vec2(200.f / 2.f, 100.f) * 1.7f;
		auto front_Hit = RESOURCE->CreateSprite(L"Player\\Front\\Front_Hit.png", hitSize, Int2(2, 1), false);
		front_Hit->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Hit][EPlayerDir::Front] = front_Hit;

		auto back_Hit = RESOURCE->CreateSprite(L"Player\\Back\\Back_Hit.png", hitSize, Int2(2, 1), false);
		back_Hit->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Hit][EPlayerDir::Back] = back_Hit;

		auto left_Hit = RESOURCE->CreateSprite(L"Player\\Left\\Left_Hit.png", hitSize, Int2(2, 1), false);
		left_Hit->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Hit][EPlayerDir::Left] = left_Hit;
		m_Sprites[ECharacterState::Hit][EPlayerDir::Front_Left] = left_Hit;
		m_Sprites[ECharacterState::Hit][EPlayerDir::Back_Left] = left_Hit;

		auto right_Hit = RESOURCE->CreateSprite(L"Player\\Right\\Right_Hit.png", hitSize, Int2(2, 1), false);
		right_Hit->SetParentRT(*m_Collision);
		m_Sprites[ECharacterState::Hit][EPlayerDir::Right] = right_Hit;
		m_Sprites[ECharacterState::Hit][EPlayerDir::Front_Right] = right_Hit;
		m_Sprites[ECharacterState::Hit][EPlayerDir::Back_Right] = right_Hit;
	}

	// Hit Blur
	{
		m_Player_HitBlur = RESOURCE->CreateUI(L"UI\\CameraHurtEffect.png", Vec2(WINCX, WINCY), false);
		m_Player_HitBlur->color.A(0.f);
	}

	// Start Image Setting
	m_CurImg = m_Sprites[ECharacterState::Idle][EPlayerDir::Front];
	SetSprite(m_State->Get_State(), m_State->Get_Dir());
}

void CPlayer::Print_ImGui()
{
#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		float hp = m_Status->GetHp();
		float mp = m_Status->GetMp();
		int gold = m_Status->GetGold();
		float speed = m_Status->GetSpeed();

		float attack = m_Status->GetAttack();
		float Def = m_Status->GetDef();

		ImGui::InputFloat("Hp", &hp);
		m_Status->SetHp(hp);

		ImGui::InputFloat("Mp", &mp);
		m_Status->SetMp(mp);

		ImGui::InputFloat("Attack", &attack);
		m_Status->SetAttack(attack);

		ImGui::InputFloat("Def", &Def);
		m_Status->SetDef(Def);

		//ImGui::InputFloat("Speed", &speed);
		//m_Status->SetSpeed(speed);
		//
		ImGui::InputInt("Gold", &gold);
		m_Status->SetGold(gold);

		ImGui::Text("State : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
		ImGui::Text("Dir   : %s", CHelper::ToString_PlayerDir(m_State->Get_Dir()));
		ImGui::Text("LastDir.x : %f", m_LastDir.x);
		ImGui::Text("LastDir.y : %f", m_LastDir.y);

		// 스프라이트 세팅
		if (m_CurImg)
		{
			Vec2 localPos = m_CurImg->GetLocalPos();
			ImGui::InputFloat2("Sprite Pos", &localPos.x);
			m_CurImg->SetLocalPos(localPos);
		}

		// 위치 세팅
		Vec2 worldPos = m_Collision->GetWorldPos();
		if (ImGui::InputFloat2("Player Pos", &worldPos.x))
		{
			m_Collision->SetWorldPos(worldPos);
		}
	}

	ImGui::End();

#endif
}

void CPlayer::MoveTile(const Vec2& dir, float speed)
{
	CCharacter::MoveTile(dir, speed);

}

void CPlayer::ResetSpeed()
{
	m_WalkTimer = 0.f;
	m_Status->SetSpeed(400.f);
}

void CPlayer::SwapKeepFrame(ECharacterState state, EPlayerDir newDir)
{
	auto& map = m_Sprites[state];
	auto it = map.find(newDir);
	if (it == map.end() || m_CurImg == it->second) return;

	Int2 curFrame = m_CurImg->frame;
	ANIMSTATE curAnim = m_CurImg->Get_AnimState();

	m_CurImg->visible = false;
	m_CurImg = it->second;
	m_CurImg->visible = true;

	if (m_Weapon && m_Weapon->IsFiringSkill())
		m_CurImg->ChangeAnim(curAnim, 0.048f);
	else
		m_CurImg->ChangeAnim(curAnim, 0.06f);

	curFrame.x = clamp(curFrame.x, 0, m_CurImg->maxFrame.x - 1);
	curFrame.y = clamp(curFrame.y, 0, m_CurImg->maxFrame.y - 1);
	m_CurImg->frame = curFrame;

	Shadow_Size();

}
