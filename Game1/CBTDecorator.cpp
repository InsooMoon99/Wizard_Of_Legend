#include "stdafx.h"
#include "CBTDecorator.h"

BTResult CBTInverter::Tick(BTContext& ctx)
{
	if (!m_Child) 
		return BTResult::Failure;

	auto result = m_Child->Tick(ctx);

	// Success <-> Failure 반전시키기
	if (result == BTResult::Success)
		return BTResult::Failure;

	if (result == BTResult::Failure)
		return BTResult::Success;

	return BTResult::Running;
}
