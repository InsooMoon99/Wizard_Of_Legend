#pragma once

#include "CBTNode.h"

class CBTService
{
public:
	using Fn = function<void(BTContext&)>;

	explicit CBTService(float periodSec = 0.2f) : m_Period(periodSec) {}
	
	void SetOnTick(Fn fn)		{ m_OnTick = move(fn); }
	void SetPeriod(float sec)	{ m_Period = (sec < 0.f ? 0.f : sec); }
	void Reset()				{ m_Timer = 0.f; }

	void Tick(BTContext& ctx, float delta);

private:
	float	m_Period = 0.2f;
	float	m_Timer = 0.f;
	Fn		m_OnTick = nullptr;

};

