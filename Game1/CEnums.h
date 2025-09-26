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
	Hit,

	Air,
	Boss_Start,
	Boss_Start2,
	Boss_Ready,
	Boss_Attack1,
	Boss_Attack1_Ready,
	Boss_Attack2,
	Boss_Attack3,

	Boss_Teleport,

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

	Boss_Man_Bat,
	Boss_Girl_Dash,

	Boss_Man_Target,
	Boss_Girl_Javelin,

	Boss_Man_Star,
	Boss_Girl_Star,

	SKILL_END
};

enum class ESkillActor
{
	Dash_Shock,
	Star_Collision,


	SKILL_END
};

enum class EBossType
{
	Twin_Man,
	Twin_Girl,

	BT_END
};

enum class EBossState
{
	Idle,

	Boss_Start,
	Boss_Start2,
	Boss_Attack1,
	Boss_Attack2,

	BS,END
};

enum class EStarType
{
	First,
	Second,
	Third,

	END
};

enum ESkillSlot
{
	LButton,
	Space,
	RButton,
	Q,
	E,
	R,

	END
};