#include "stdafx.h"
#include "CSkill.h"
#include "CPlayer.h"

CSkill::CSkill()
{

}

CSkill::~CSkill()
{

}

void CSkill::BeginPlay()
{

}

void CSkill::Tick()
{
	if (m_Icon)
		m_Icon->Update();

	if (m_CoolRemain > 0.f)
	{
		m_CoolRemain -= DELTA;

		if (m_CoolRemain < 0.f)
			m_CoolRemain = 0.f;
	}
}

void CSkill::LateUpdate()
{
}

void CSkill::Release()
{
}

void CSkill::Render()
{
	if (m_Icon)
		m_Icon->Render();
}

bool CSkill::CanActivate(const FSkillInfo& _Info)
{
	if (m_CoolRemain > 0.f)
		return false;

	return true;
}

void CSkill::Activate(CActor* _Owner)
{
	

}

void CSkill::SetIcon(const wstring& path, const Vec2& size)
{
	if (m_Icon == nullptr)
	{
		m_Icon = RESOURCE->CreateUI(path, size, false);
		m_Icon->pivot = OFFSET_LT;
	}
}
