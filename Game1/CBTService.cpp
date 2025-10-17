#include "stdafx.h"
#include "CBTService.h"

void CBTService::Tick(BTContext& ctx, float delta)
{
	m_Timer += delta;

	if (m_Timer >= m_Period)
	{
		m_Timer -= m_Period;

		OnTick(ctx);
	}
}
