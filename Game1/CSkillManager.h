#pragma once
#include "CStruct.h"

class CSkill_Actor;

class CSkillManager
{
private:
	CSkillManager() = default;
	~CSkillManager() = default;

public:
	void		Init();
	void		Update();
	void		Render();
	void		Release();

public:
	static CSkillManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CSkillManager();
		}

		return Instance;
	}

	static void Destroy_Instance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static CSkillManager* Instance;

public:
	void SkillSettings();

	shared_ptr<CSkill_Actor> Spawn_SkillActor(ESkillActor _Skill, FSkillInfo& _Info, const Vec2& size = Vec2(0.f));
	shared_ptr<CSkill_Actor> Spawn_SkillActor(ESkillActor _Skill, FSkillInfo& _Info, EStarType _Star);

private:
	unordered_map<wstring, shared_ptr<CSkill_Actor>> m_Skills;
	vector<shared_ptr<CSkill_Actor>> m_ActiveSkills;
};

