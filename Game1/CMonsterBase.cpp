#include "stdafx.h"
#include "CMonsterBase.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"

CMonsterBase::CMonsterBase()
{
	
}

CMonsterBase::~CMonsterBase()
{
	Release();

}

void CMonsterBase::BeginPlay()
{
	CCharacter::BeginPlay();


}

void CMonsterBase::Tick()
{
	CCharacter::Tick();


}

void CMonsterBase::LateUpdate()
{
	CCharacter::LateUpdate();

}

void CMonsterBase::Render()
{
	CCharacter::Render();

	

}

void CMonsterBase::Release()
{
	CCharacter::Release();

}