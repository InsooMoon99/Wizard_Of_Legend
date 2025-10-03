#include "stdafx.h"
#include "CUI_MiddleBossStatus.h"

#include "CStatusComponent.h"

CUI_MiddleBossStatus::CUI_MiddleBossStatus()
{
	m_Background = RESOURCE->CreateUI(L"UI\\BOSS_HP_BACK.png", Vec2(114.f, 16.f) * 2.8f, true);
	m_Background->SetWorldPos(Vec2(0.f, 280.f));
	m_Background->collider = COLLIDER::NONE;
	m_Background->space = SPACE::SCREEN;

	m_Hp = RESOURCE->CreateUI(L"UI\\BOSS_HPBAR.png", Vec2(m_HpScale.x, m_HpScale.y), true);
	m_Hp->SetParentRT(*m_Background);
	m_Hp->SetLocalPos(Vec2(-123.5f, -1.8f));
	m_Hp->pivot = OFFSET_L;
	m_Hp->collider = COLLIDER::NONE;
	m_Hp->space = SPACE::SCREEN;

	m_Hp_back = RESOURCE->CreateUI(L"UI\\HPBarHurtFill.png", Vec2(m_HpScale.x, m_HpScale.y), true);
	m_Hp_back->SetParentRT(*m_Background);
	m_Hp_back->SetLocalPos(Vec2(-123.5f, -1.8f));
	m_Hp_back->pivot = OFFSET_L;
	m_Hp_back->collider = COLLIDER::NONE;
	m_Hp_back->space = SPACE::SCREEN;

	m_BossStatus.push_back(m_Background);
	m_BossStatus.push_back(m_Hp_back);
	m_BossStatus.push_back(m_Hp);
}

CUI_MiddleBossStatus::~CUI_MiddleBossStatus()
{
	Release();
}

void CUI_MiddleBossStatus::BeginPlay()
{
	CUI::BeginPlay();
}

void CUI_MiddleBossStatus::Tick()
{
	CUI::Tick();

	if (m_Status)
	{
		const float target = max(0.f, min(1.f, m_Status->GetHp() / m_Status->GetMaxHp()));

		m_InstantRatio = target;
		m_Hp->scale.x = m_HpScale.x * m_InstantRatio;

		if (m_HurtRatio > target)
		{
			float t = DELTA * m_HurtLerpSpeed;             
			if (t > 1.f) t = 1.f;
			m_HurtRatio = m_HurtRatio + (target - m_HurtRatio) * t;

			if (fabsf(m_HurtRatio - target) < 0.002f)
				m_HurtRatio = target;
		}
		else
		{
			m_HurtRatio = target;
		}

		m_Hp_back->scale.x = m_HpScale.x * m_HurtRatio;
	}

	for (auto& ui : m_BossStatus)
		ui->Update();


	RENDER->AddRenderObject(this);
}

void CUI_MiddleBossStatus::Render()
{
	CUI::Render();

	//#ifdef _DEBUG
	//	if (ImGui::Begin("Debug"))
	//	{
	//		Vec2 pos = m_Background->GetLocalPos();
	//		ImGui::InputFloat2("BossBack", &pos.x);
	//	
	//		m_Background->SetLocalPos(pos);
	//	}
	//	
	//	ImGui::End();
	//#endif


	for (auto& status : m_BossStatus)
	{
		status->Render();
	}
}

void CUI_MiddleBossStatus::Release()
{
	CUI::Release();

	m_BossStatus.clear();
}
