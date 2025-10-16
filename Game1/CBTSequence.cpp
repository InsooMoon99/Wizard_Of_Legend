#include "stdafx.h"
#include "CBTSequence.h"

BTResult CBTSequence::Tick(BTContext& ctx)
{
	TickServices(ctx, DELTA);

	for (int32 i = m_RunningIdx; i < m_Children.size(); i++)
	{
		auto result = m_Children[i]->Tick(ctx);

		if (result == BTResult::Running)
		{
			m_RunningIdx = i;

			return BTResult::Running;
		}

		if (result == BTResult::Failure)
		{
			m_RunningIdx = 0;

			return BTResult::Failure;
		}
	}

	// 모두 성공, Success 반환
	m_RunningIdx = 0;
	return BTResult::Success;

}
