#pragma once

class CHelper
{
public:
	static const char* ToString_PlayerState(PlayerState _State)
	{
		switch (_State)
		{
		case PlayerState::Idle:   return "Idle";
		case PlayerState::Walk:   return "Walk";
		case PlayerState::Run:    return "Run";
		case PlayerState::Attack: return "Attack";

		default:                  return "None";
		}
	}

	static const char* ToString_PlayerDir(PlayerDir _Dir)
	{
		switch (_Dir)
		{
		case PlayerDir::Front:	return "Front";
		case PlayerDir::Back:	return "Back";
		case PlayerDir::Left:	return "Left";
		case PlayerDir::Right:	return "Right";

		case PlayerDir::PD_END:	return "PD_End";
		default: return "None";
		}
	}

};