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

}

void CSkill::LateUpdate()
{
}

void CSkill::Release()
{
}

void CSkill::Render()
{

}

bool CSkill::CanActivate(const FSkillInfo& _Info)
{
	// TODO : 쿨타임만 구현하면 될거같다.

	return true;
}

void CSkill::Activate(CActor* _Owner)
{
	

}

void CSkill::SetIcon(const wstring& path, const Vec2& size)
{
	if (m_Icon == nullptr) 
		m_Icon = RESOURCE->CreateUI(path, size, true);

}
