#pragma once

#include "CCharacter.h"
#include "CEnums.h"
#include "CStruct.h"

class CComponent;
class CStateComponent;
class CWeaponComponent;
class CInventoryComponent;
class CUIComponent;
class CItem;

class CPlayer : public CCharacter
{
public:
	CPlayer();
	virtual ~CPlayer() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	// Interface
	void Update_Hit() override;
	void Hit_Color() override;
	void Hit_Effect() override;

	void TakeDamage(FHitDamage _Hit) override;

private:
	// Components
	shared_ptr<CWeaponComponent>		m_Weapon;
	shared_ptr<CInventoryComponent>		m_Inven;
	shared_ptr<CUIComponent>			m_UI;

public:
	void Key_Input();

	void State_Idle();
	void State_Walk();
	void State_Dash();
	void State_Attack();
	void State_Attack3();
	void State_Hit();

	virtual void SetSprite(ECharacterState _State, EPlayerDir _Dir) override;
	virtual void Play_Sprite(ECharacterState _State);

public:
	void Attack_Sprite();
	void Shadow_Size();
	void Dash_Sprite();

	void AddInteract(shared_ptr<CItem> _InItem);
	void RemoveInteract(CItem* _InItem);
	void UseItem();

	void SwapKeepFrame(ECharacterState state, EPlayerDir newDir);
	
protected:
	void Resource_Settings() override;

private:
	void Print_ImGui();
	void MoveTile(const Vec2& dir, float speed);
	void ResetSpeed();

	float LastDirRotation();

private:
	Vec2 m_MoveDir = { 0, 0 };
	Vec2 m_LastDir = { 0, -1.f };

	float m_DashDistance = 1200.f;

	// Image
	unordered_map<ECharacterState, shared_ptr<ObImage>> m_Shadow;
	unordered_map<EPlayerDir, shared_ptr<ObImage>>   m_Dash;

	shared_ptr<ObImage> m_Player_Marker = nullptr;
	shared_ptr<ObImage> m_Player_HitBlur = nullptr;

private:
	Vec2	m_StartShadowScale;
	Vec2	m_EndShadowScale;

	float   m_WalkTimer = 0.f;
	float	m_DashTimer = 0.4f;
	int		m_DashEndFrame = 11;

	vector<weak_ptr<CItem>> m_Interact;
	weak_ptr<CItem> m_CurItem;

	float m_HitBlurAlpha = 0.f;
	float m_HitBlurFadeIn = 0.06f;   
	float m_HitBlurFadeOut = 0.10f;  
	float m_HitBlurMaxAlpha = 0.35f; 
	bool  m_HitBlurWantsOn = false;

	float m_AttackMoveRatio = 0.25f;

	bool  m_SkillMove = false;

};



