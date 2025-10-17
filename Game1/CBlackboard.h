#pragma once

#include "CActor.h"

namespace BBKey
{
	const string Target = "Target";
	const string ToTargetDist = "ToTargetDist";
}

class CBlackboard : public CActor
{
public:
	using Key = string;

public:
	void SetValueAsActor(const Key& key, CActor* actor) { m_Actors[key] = actor; }
	void SetValueAsFloat(const Key& key, float value)	{ m_Floats[key] = value; }
	void SetValueAsInt(const Key& key, int value)		{ m_Ints[key] = value; }
	void SetValueAsBool(const Key& key, bool flag)		{ m_Bools[key] = flag; }
	void SetValueAsVector2(const Key& key, Vec2 value)	{ m_Vec2[key] = value; }
	void SetValueAsVector3(const Key& key, Vec3 value)	{ m_Vec3[key] = value; }

public:
	// Get : 키가 없다면 기본값을 반환하도록?
	CActor* GetValueAsActor(const Key& key, CActor* _default = nullptr);
	float	GetValueAsFloat(const Key& key, float _default = 0.f);
	int		GetValueAsInt(const Key& key, int _default = 0);
	bool	GetValueAsBool(const Key& key, bool _default = false);
	Vec2	GetValueAsVector2(const Key& key, Vec2 _default = Vec2(0.f, 0.f));
	Vec3	GetValueAsVector3(const Key& key, Vec3 _default = Vec3(0.f, 0.f, 0.f));

	// Has : 키가 있는지 확인
	bool HasObject(const Key& key) const	{ return m_Actors.count(key) > 0; }
	bool HasFloat(const Key& key) const		{ return m_Floats.count(key) > 0; }
	bool HasInt(const Key& key) const		{ return m_Ints.count(key) > 0; }
	bool HasBool(const Key& key) const		{ return m_Bools.count(key) > 0; }
	bool HasVector2(const Key& key) const	{ return m_Vec2.count(key) > 0; }
	bool HasVector3(const Key& key) const	{ return m_Vec3.count(key) > 0; }

	void ClearKey(const Key& k)
	{
		m_Actors.erase(k); m_Floats.erase(k); m_Ints.erase(k);
		m_Bools.erase(k);   m_Vec2.erase(k);   m_Vec3.erase(k);
	}

	void ClearAll()
	{
		m_Actors.clear(); m_Floats.clear(); m_Ints.clear();
		m_Bools.clear();   m_Vec2.clear();   m_Vec3.clear();
	}

private:
	unordered_map<Key, CActor*> m_Actors;
	unordered_map<Key, float>	m_Floats;
	unordered_map<Key, int>		m_Ints;
	unordered_map<Key, bool>	m_Bools;
	unordered_map<Key, Vec2>	m_Vec2;
	unordered_map<Key, Vec3>	m_Vec3;

};



