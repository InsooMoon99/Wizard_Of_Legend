#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "json.hpp"

class ObImage;

class CItemDataManager
{
private:
    CItemDataManager() = default;
    ~CItemDataManager() = default;
    CItemDataManager(const CItemDataManager&) = delete;
    CItemDataManager& operator=(const CItemDataManager&) = delete;

public:
    static CItemDataManager* Get_Instance()
    {
        static CItemDataManager s; return &s;
    }

    bool Init(const wstring& path = L"Item_Data.json"); 
    bool Reload();                                           
    void Clear();                                            

    const FItemData* GetById(const string& id) const;
    shared_ptr<ObImage> GetIcon(const string& id);

    vector<const FItemData*> GetAll() const;

private:
    using json = nlohmann::json;

    bool LoadFromFile(const wstring& path);
    bool Parse(const json& root);

    // 문자열 -> enum
    static bool ParseItemType(const string& s, EItemType& out);
    static bool ParseStatType(const string& s, EItem_StatType& out);
    static bool ParseOption(const string& s, EModifyOption& out);

    static wstring Utf8ToW(const string& s);
    static int   GetInt(const json& obj, const char* key, int def = 0);
    static float GetFlt(const json& obj, const char* key, float def = 0.f);

private:
    // Id 조회해서 Data
    unordered_map<string, FItemData> m_Table;
    // IconKey 찾아서 이미지 세팅 Image
    unordered_map<string, shared_ptr<ObImage>> m_IconCache;

    wstring m_LastPath;
};
