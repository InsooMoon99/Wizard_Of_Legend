#include "stdafx.h"
#include "CBoss_Controller.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"
#include "CUIComponent.h"

CBoss_Controller::CBoss_Controller()
{
	m_Status = CHelper::CreateSharedComponent<CStatusComponent>(this);
	m_State = CHelper::CreateSharedComponent<CStateComponent>(this);
	m_UI = CHelper::CreateSharedComponent<CUIComponent>(this);
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
		m_Status->SetHp(800.f); 
	}

	if (m_UI)
	{
		m_UI->SetBossStatus();
	}
}

void CBoss_Controller::Tick()
{
	CActor::Tick();
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

void CBoss_Controller::TakeDamage(float _Damage)
{
	if (!m_Status) 
		return;

	m_Status->SetHp(m_Status->GetHp() - _Damage);
}
