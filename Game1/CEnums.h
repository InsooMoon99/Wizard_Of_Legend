#pragma once

enum class SceneType
{
	Start = 0,
	Stage01,
	Stage02,

	Stage_End
};

enum class LayerType
{
	Layer_BackGround,
	Layer_Object,
	//...
	Layer_UI,

	Layer_MaxCount,
};

enum class PlayerState
{
	Idle = 0,
	Walk,
	Run,
	Attack,

	PS_END
};

enum class PlayerDir
{
	Front,
	Back,
	Left,
	Right,

	PD_END
};