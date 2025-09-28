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

struct FHitDamage
{
	float Damage = 0.f;

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

