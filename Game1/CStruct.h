#pragma once

struct SkillInfo
{
	CActor*	Instigator = nullptr;
	Vec2	SpawnPos {};
	Vec2	Aim_Dir	 {};
	float	Amount	 {};

	float	Angle = 0.f;
	float	Damage = 0.f;
	float	Mp = 0.f;
};
