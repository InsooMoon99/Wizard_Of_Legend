#include "stdafx.h"
#include "CBoss_Controller.h"

#include "CBossPawn.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"
#include "CUIComponent.h"

CBoss_Controller::CBoss_Controller()
{
	m_Status = CHelper::CreateSharedComponent<CStatusComponent>(this);
	m_State = CHelper::CreateSharedComponent<CStateComponent>(this);
	m_UI = CHelper::CreateSharedComponent<CUIComponent>(this);

	//m_Status->SetMaxHp(500.f);
	//m_Status->SetHp(500.f);

	m_State->Change_State(ECharacterState::Boss_Wait);
}

CBoss_Controller::~CBoss_Controller()
{
	Release();
}

void CBoss_Controller::BeginPlay()
{
	CActor::BeginPlay();

	// HP 초기 세팅
	if (m_Status)
	{
		m_Status->SetMaxHp(1000.f); 
		m_Status->SetHp(1000.f);
	}

	if (m_UI)
		m_UI->SetBossHUD();

	OnBossStateChanged.Add([this](ECharacterState state)
		{
			m_State->Change_State(state);
		});

	OnBossStart.Add([this]()
		{
			m_ReadyCount++;
			if (m_ReadyCount >= 2)
			{
				ChangeState(ECharacterState::Boss_Start2); 
				m_ReadyCount = 0;
			}
		});

	OnBossStateChanged.Broadcast(m_State->Get_State());
}

void CBoss_Controller::Tick()
{
	CActor::Tick();

	if (m_State->Get_State() == ECharacterState::Idle)
	{
		m_AttackTimer -= DELTA;

		if (m_AttackTimer <= 0.f)
		{
			m_State->Change_State(ECharacterState::Boss_Ready);

			m_AttackTimer = 0.5f;
		}
	}

	m_ManPos = m_Pawns[(int)EBossType::Twin_Man]->Get_WorldPos();
	m_GirPos = m_Pawns[(int)EBossType::Twin_Girl]->Get_WorldPos();

	Vec2 centerPos = (m_ManPos + m_GirPos) / 2.0f;
	m_OffsetPos = (m_GirPos - m_ManPos) / 2.0f;
}

void CBoss_Controller::LateUpdate()
{
	CActor::LateUpdate();

}

void CBoss_Controller::Render()
{
	CActor::Render();

#ifdef _DEBUG
	if (ImGui::Begin("Debug"))
	{
		ImGui::NewLine();

		float hp = m_Status->GetHp();

		ImGui::InputFloat("Boss Hp", &hp);
		m_Status->SetHp(hp);

		ImGui::Text("Constroller : %s", CHelper::ToString_PlayerState(m_State->Get_State()));
	}

	ImGui::End();

#endif
}

void CBoss_Controller::Release()
{
	CActor::Release();

	CActor::Release();

	m_Pawns.clear();

	m_Status.reset();
	m_State.reset();
	m_UI.reset();
}

void CBoss_Controller::RegisterPawn(CBossPawn* _Pawn)
{
	if (_Pawn)
	{
		m_Pawns.push_back(_Pawn);
	}
}

void CBoss_Controller::UnRegisterPawn(CBossPawn* _Pawn)
{
	auto find_iter = find(m_Pawns.begin(), m_Pawns.end(), _Pawn);
	if (find_iter != m_Pawns.end())
	{
		m_Pawns.erase(find_iter);
	}
}

void CBoss_Controller::TakeDamage(float rawDamage, const Vec2& worldPos)
{
	if (!m_Status) return;
	if (m_State->Get_State() == ECharacterState::Dead) return;

	float damage = CHelper::CaculateDamage(rawDamage, m_Status->GetDef());
	CHelper::ApplyDamage(m_Status, damage, worldPos);   

	int random = RANDOM->Int(1, 4);

	switch (random)
	{
	case 1:
		SOUND->StopPlay("BHurt1", 0.1f);
		break;

	case 2:
		SOUND->StopPlay("BHurt2", 0.1f);
		break;

	case 3:
		SOUND->StopPlay("BHurt3", 0.1f);
		break;

	case 4:
		SOUND->StopPlay("BHurt4", 0.1f);
		break;

	}

	if (m_Status->GetHp() <= 0.f)
	{
		PARTICLE->Spawn_Particle(L"Enhance", worldPos);

		ChangeState(ECharacterState::Dead);
		
	}
}

void CBoss_Controller::ChangeState(ECharacterState _State)
{
	if (!m_State)
		return;

	m_State->Change_State(_State);

	OnBossStateChanged.Broadcast(_State);
}
