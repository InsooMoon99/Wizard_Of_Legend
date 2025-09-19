#pragma once

enum class ESceneType
{
	Start = 0,
	Stage01,
	Stage02,

	Edit,
	Shadow,
	Stage_End
};

// TODO : 렌더링 순서 문제 생기면 ELayerType 사용하기
enum class ELayerType
{
	Layer_BackGround,
	Layer_Object,
	Layer_Player,
	Layer_UI,

	Layer_MaxCount,
};

enum class EPlayerState
{
	Idle = 0,
	Walk,
	Dash,
	Attack1,
	Attack2,

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
	Wind_Slash,
	Fire_Ball,
	Gaia_Armor,


	SKILL_END
};
