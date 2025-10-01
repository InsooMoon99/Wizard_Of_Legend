#pragma once

#include "CCharacter.h"

class CMonsterBase : public CCharacter
{
public:
	CMonsterBase();
	virtual ~CMonsterBase();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	// Interface
	void Update_Hit() override {};
	void Hit_Color() override  {};
	void Hit_Effect() override {};

public:
	virtual void SpriteResetIdle();

protected:
	void Resource_Settings() override {};
	void SetSprite(ECharacterState _State, EPlayerDir _Dir) override {};
	void Play_Sprite(ECharacterState _State) override;

public:
	shared_ptr<CActor> GetTarget() { return m_Target; }
	void SetTarget(shared_ptr<CActor> actor) { m_Target = actor;  }

	shared_ptr<GameObject> GetSight() { return m_SightRaidus; }
	void SetSight(shared_ptr<GameObject> sight) { m_SightRaidus = sight; }

	Vec2 GetTraceDir() { return m_TraceDir; }
	void SetTraceDir(const Vec2& dir) { m_TraceDir = dir; }

	virtual void TraceTarget() {};
	virtual void DropGold();

	void TakeDamage(FHitDamage _Hit) override;

protected:
	shared_ptr<CActor>		m_Target = nullptr;
	shared_ptr<GameObject>	m_SightRaidus = nullptr;

	Vec2 m_TraceDir;

private:

};

