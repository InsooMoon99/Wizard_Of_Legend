#include "stdafx.h"
#include "CMonsterBase.h"

#include "CItem_Gold.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"
#include "CStateComponent.h"

CMonsterBase::CMonsterBase()
{

	m_RenderLayer = ERenderLayer::Render_GameObject;
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

void CMonsterBase::SpriteResetIdle()
{
	if (m_CurImg && m_CurImg->Get_AnimState() == ANIMSTATE::STOP)
	{
		m_State->Change_State(ECharacterState::Idle);
	}
}

void CMonsterBase::Play_Sprite(ECharacterState _State)
{
	CCharacter::Play_Sprite(_State);

	switch (_State)
	{
	case ECharacterState::Dead:
		DropItem();
		break;
	}
}

void CMonsterBase::DropItem()
{
	Vec2 dropPos = m_Collision->GetWorldPos();

	auto gold = make_shared<CItem_Gold>();
	gold->SetSpawnPos(dropPos);

	OBJECT->SpawnActor(gold);
}

void CMonsterBase::TakeDamage(FHitDamage _Hit)
{
	CCharacter::TakeDamage(_Hit);

	float damage = CHelper::CaculateDamage(_Hit.Damage, m_Status->GetDef());
	CHelper::ApplyDamage(m_Status, damage, m_Collision->GetWorldPos());
}
