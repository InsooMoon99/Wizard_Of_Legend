#pragma once

#include "CEnums.h"

struct FSkillInfo
{
	CActor*	Instigator = nullptr;
	CActor* Target = nullptr;
	Vec2	TargetPos {};

	Vec2	SpawnPos  {};
	Vec2	Aim_Dir	  {};
	float	Amount	  {};

	float	Angle = 0.f;
	float	Damage = 0.f;
	float	Mp = 0.f;

	int		WaveMode = 0;
	float   TravelMax = 0.f;
};

struct FSkillData
{
	string  Id;
	wstring Name = L"";
	wstring Desc = L"";

	float	Damage = 0.f;
	float	CoolDown = 0.f;

	string  IconKey; // 이미지 말고, 경로만 들고있기
	int     IconW = 0;
	int     IconH = 0;
};

struct FSkill_UiInfo
{
	shared_ptr<ObImage> Icon;

	ESkillSlot Slot;
	ESkillType Type;

	float Cooldown;
	float CooldownRemain;
	bool  IsCoolTime = false;

	string	Id;     
	wstring Name;    
	wstring Desc;
	int     Level = 1;

	bool operator==(const FSkill_UiInfo& o) const
	{
		return Slot == o.Slot
			&& Type == o.Type
			&& Icon.get() == o.Icon.get()
			&& Cooldown == o.Cooldown
			&& CooldownRemain == o.CooldownRemain;
	}

	bool operator!=(const FSkill_UiInfo& o) const { return !(*this == o); }

};

struct FItemUIInfo
{
	string		 Id;          
	wstring		 Name;       
	wstring		 Desc;        
	int          Value = 0;   
	EItemType    Type = EItemType::Etc;
	wstring		 IconPathW;   
	bool         IsEmpty = true;

};

struct FHitDamage
{
	float Damage = 0.f;
	float Amount = 0.f;
	float Duration = 0.f;
	Vec2  Dir{};

	GameObject* colldier = nullptr;
	GameObject* other = nullptr;
};

struct FItemData
{
	string		Id;
	wstring		Name;
	wstring		Desc;

	int			Price = 0;
	string		IconKey;
	int			IconW = 0;
	int			IconH = 0;

	EItemType		ItemType = EItemType::Etc;
	EItem_StatType	ItemStatType = EItem_StatType::END;
	EModifyOption	Option = EModifyOption::END;

	float Value;

};

struct FKnockbackState
{
	bool  active = false;
	Vec2  dir = { 0,0 };
	float t = 0.f;
	float duration = 0.f;
	float amount = 0.f; 
};

struct FFloatingTextDesc
{
	wstring		 text;
	wstring		 fontStyle = L"Noto Sans Cond Med";
	Vec2         startPos;     
	float        duration = 0.8f;
	float        rise = 60.f;   
	float        fontSize = 28.f;
	Color        color = Color(1, 0.9f, 0.2f, 1); 

	bool  outline = false;
	float outlineThickness = 1.5f;                 
	Color outlineColor = Color(0, 0, 0, 0.85f);    
};

struct FMeleeParams
{
	float moveStart = 0.15f;
	float moveEnd   = 1.15f;
	float moveDist  = 120.f;

	float hitTime  = 0.20f;
	float duration = 0.60f;

	float damage	 = 30.f;
	Vec2  hitBoxSize = Vec2(80.f, 40.f);

	string AttackSound = "None";
};
