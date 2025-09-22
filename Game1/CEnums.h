#pragma once

enum class ERenderLayer
{
	Render_Background,
	Render_GameObject,
	Render_Effect,
	Render_UI,

	Render_END
};

enum class ESceneType
{
	Start = 0,
	Stage01,
	Stage02,

	Edit,
	Shadow,
	Stage_End
};

enum class ECharacterState
{
	Idle = 0,
	Walk,
	Dash,
	Attack1,
	Attack2,
	Attack_FireDragon,

	Boss_Start,

	PS_END
};

enum class EPlayerDir
{
	Front,
	Back,
	Left,
	Right,

	Front_Left,
	Front_Right,
	Back_Left,
	Back_Right,

	PD_END
};

enum class ETileType
{
	WALL,
	TILE2,
	FALL,

	TILE_END
};

enum class ESkillType
{
	Fire_Ball,
	Fire_Dragon,

	Wind_Slash,

	Earth_Shield,


	SKILL_END
};
