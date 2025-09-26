#pragma once

#include "CSkill.h"
#include <memory>

class CSkillDataManager
{
private:
	CSkillDataManager() = default;
    ~CSkillDataManager() = default;

	CSkillDataManager(const CSkillDataManager&) = delete;
	CSkillDataManager& operator=(const CSkillDataManager&) = delete;

public:
	void Init();

private:
    wstring ConvertUtf8ToWString(const string& utf8_str);

public:
	static CSkillDataManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CSkillDataManager();
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

    bool LoadData(const wstring& filename);

    // 데이터 접근 함수
    const FSkillData* GetSkillData(const wstring& name) const;

private:
	static CSkillDataManager* Instance;
	map<wstring, FSkillData> m_SkillTable;
};