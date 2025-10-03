#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "CStruct.h"

class ObImage;

class CSkillDataManager
{
private:
    CSkillDataManager() = default;
    ~CSkillDataManager() = default;
    CSkillDataManager(const CSkillDataManager&) = delete;
    CSkillDataManager& operator=(const CSkillDataManager&) = delete;

public:
    static CSkillDataManager* Get_Instance()
	{
        if (!Instance) Instance = new CSkillDataManager();
        return Instance;
    }
    static void Destroy_Instance()
    {
	    delete Instance;
    	Instance = nullptr;
    }

public:
    void Init();                                     
    bool LoadData(const wstring& filename);          
    void ClearCache();                               

    const FSkillData* GetById(const string& id) const;
    shared_ptr<ObImage> GetIconById(const string& id);

private:
    static wstring ConvertUtf8ToWString(const string& s);

private:
    static CSkillDataManager* Instance;

    unordered_map<string, FSkillData> m_ById;
    unordered_map<string, shared_ptr<ObImage>> m_IconCache;
};
