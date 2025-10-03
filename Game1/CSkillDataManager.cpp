#include "stdafx.h"
#include "CSkillDataManager.h"
#include "CResourceManager.h"   
#include <fstream>
#include <iostream>
#include "json.hpp"

using nlohmann::json;

CSkillDataManager* CSkillDataManager::Instance = nullptr;

void CSkillDataManager::Init()
{
    if (LoadData(L"Skill_Data.json"))
        cout << "Skill Load Success\n";
    else
        wcerr << L"Skill Data Load Fail.\n";
}

wstring CSkillDataManager::ConvertUtf8ToWString(const string& s)
{
    if (s.empty()) return L"";
    int lenW = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    if (lenW <= 0) return L"";
    wstring w(lenW, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), w.data(), lenW);
    return w;
}

bool CSkillDataManager::LoadData(const wstring& filename)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        wcerr << L"Error: open fail: " << filename << L"\n";
        return false;
    }

    json j;
    try { file >> j; }
    catch (...) {
        wcerr << L"Error: JSON parse fail (UTF-8로 저장 확인).\n";
        return false;
    }

    if (!j.contains("Skill_Data") || !j["Skill_Data"].is_array()) {
        wcerr << L"Error: 'Skill_Data' array missing.\n";
        return false;
    }

    m_ById.clear();
    m_IconCache.clear(); // 데이터 바뀌면 아이콘 다시 로드

    for (auto& row : j["Skill_Data"])
    {
        FSkillData data{};

        data.Id = row.value("Id", "");
        data.Name = ConvertUtf8ToWString(row.value("Name", ""));
        data.Desc = ConvertUtf8ToWString(row.value("Desc", ""));

        // 데미지
        if (row.contains("Damage")) 
        {
            if (row["Damage"].is_number())
                data.Damage = (float)row["Damage"].get<double>();

            else if (row["Damage"].is_string())
            {
                try
                {
	                data.Damage = (float)stod(row["Damage"].get<string>());
                }

                catch (...)
                {
	                data.Damage = 0.f;
                }
            }
        }

        if (row.contains("CoolDown"))
        {
            if (row["CoolDown"].is_number())
                data.CoolDown = (float)row["CoolDown"].get<double>();

            else if (row["CoolDown"].is_string())
            {
                try 
                {
                    data.CoolDown = (float)stod(row["CoolDown"].get<string>());
                }
                catch (...)
                {
                    data.CoolDown = 0.f;
                }
            }
        }

        data.IconKey = row.value("IconKey", "");
        data.IconW = row.value("IconW", 0);
        data.IconH = row.value("IconH", 0);

        if (!data.Id.empty())
            m_ById[data.Id] = move(data);
    }

    return true;
}

void CSkillDataManager::ClearCache()
{
    m_IconCache.clear();
}

const FSkillData* CSkillDataManager::GetById(const string& id) const
{
    auto it = m_ById.find(id);
    return (it == m_ById.end()) ? nullptr : &it->second;
}

shared_ptr<ObImage> CSkillDataManager::GetIconById(const string& id)
{
    const FSkillData* data = GetById(id);
    if (!data || data->IconKey.empty()) return nullptr;

    auto hit = m_IconCache.find(data->IconKey);
    if (hit != m_IconCache.end()) return hit->second;

    // 로드
    wstring pathW = ConvertUtf8ToWString(data->IconKey);
    Vec2 size = (data->IconW > 0 && data->IconH > 0) ? Vec2((float)data->IconW, (float)data->IconH)
        : Vec2(40.f, 40.f);

    auto icon = RESOURCE->CreateUI(pathW, size, true);
    m_IconCache[data->IconKey] = icon;

    return icon;
}
