#include "stdafx.h"
#include "CBTSelector.h"

BTResult CBTSelector::Tick(BTContext& ctx)
{
	TickServices(ctx, DELTA);

	for (int32 i = 0; i < m_RunningIdx; i++)
	{
		auto result = m_Children[i]->Tick(ctx);

		if (result == BTResult::Running)
		{
			m_RunningIdx = i;

			return BTResult::Running;
		}

		if (result == BTResult::Success)
		{
			m_RunningIdx = 0;

			return BTResult::Success;
		}
	}

	// 모두 실패하면 Failure
	m_RunningIdx = 0;

	return BTResult::Failure;
}
