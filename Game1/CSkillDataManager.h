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
    static CSkillDataManager* Get_Instance() {
        if (!Instance) Instance = new CSkillDataManager();
        return Instance;
    }
    static void Destroy_Instance() { delete Instance; Instance = nullptr; }

public:
    void Init();                                           // 시작 시 1회 호출
    bool LoadData(const wstring& filename);           // JSON 로드(UTF-8)
    void ClearCache();                                     // 아이콘 캐시 비우기(핫리로드 시)

    // === Id 기반 조회 ===
    const FSkillData* GetById(const string& id) const;
    shared_ptr<ObImage> GetIconById(const string& id);

private:
    // UTF-8 → wstring
    static wstring ConvertUtf8ToWString(const string& s);

private:
    static CSkillDataManager* Instance;

    // Id → 데이터
    unordered_map<string, FSkillData> m_ById;

    // IconKey → 아이콘 핸들 캐시
    unordered_map<string, shared_ptr<ObImage>> m_IconCache;
};
