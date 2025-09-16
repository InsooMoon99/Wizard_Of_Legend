#pragma once

struct SkillInfo
{
	CActor*	Owner = nullptr;
	Vec2	SpawnPos {};
	Vec2	Dir {};

	float	Damage = 0.f;
	float	Mp = 0.f;
};
