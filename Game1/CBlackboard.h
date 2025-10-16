#pragma once

#include "CActor.h"

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


private:
	unordered_map<Key, CActor*> m_Actors;
	unordered_map<Key, float> m_Floats;
	unordered_map<Key, int> m_Ints;
	unordered_map<Key, bool> m_Bools;
	unordered_map<Key, Vec2> m_Vec2;
	unordered_map<Key, Vec3> m_Vec3;

};

