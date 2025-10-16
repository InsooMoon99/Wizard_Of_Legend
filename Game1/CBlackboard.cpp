#include "stdafx.h"
#include "CBlackboard.h"


CActor* CBlackboard::GetValueAsActor(const Key& key, CActor* _default)
{
	auto iter = m_Actors.find(key);

	if (iter == m_Actors.end())
	{
		return _default;
	}

	return iter->second;
}

float CBlackboard::GetValueAsFloat(const Key& key, float _default)
{
	auto iter = m_Floats.find(key);

	if (iter == m_Floats.end())
	{
		return _default;
	}

	return iter->second;
}

int CBlackboard::GetValueAsInt(const Key& key, int _default)
{
	auto iter = m_Ints.find(key);

	if (iter == m_Ints.end())
	{
		return _default;
	}

	return iter->second;
}

bool CBlackboard::GetValueAsBool(const Key& key, bool _default)
{
	auto iter = m_Bools.find(key);

	if (iter == m_Bools.end())
	{
		return _default;
	}

	return iter->second;
}

Vec2 CBlackboard::GetValueAsVector2(const Key& key, Vec2 _default)
{
	auto iter = m_Vec2.find(key);

	if (iter == m_Vec2.end())
	{
		return _default;
	}

	return iter->second;
}

Vec3 CBlackboard::GetValueAsVector3(const Key& key, Vec3 _default)
{
	auto iter = m_Vec3.find(key);

	if (iter == m_Vec3.end())
	{
		return _default;
	}

	return iter->second;
}
