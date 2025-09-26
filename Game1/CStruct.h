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
	wstring Name = L"";
	wstring Desc = L"";

	float	Damage = 0.f;
};

struct FSkill_UiInfo
{
	shared_ptr<ObImage> Icon;

	ESkillSlot Slot;
	ESkillType Type;

	float Cooldown;
	float CooldownRemain;

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

