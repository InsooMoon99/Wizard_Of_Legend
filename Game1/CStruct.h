#pragma once

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

struct FHitDamage
{
	float Damage = 0.f;

	GameObject* colldier = nullptr;
	GameObject* other = nullptr;
};
