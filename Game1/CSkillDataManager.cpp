#include "stdafx.h"
#include "CSkillDataManager.h"

CSkillDataManager* CSkillDataManager::Instance = nullptr;

void CSkillDataManager::Init()
{
    // 데이터 세팅
    if (LoadData(L"Skill_Data.json"))
    {
        cout << "Skill Load Success" << endl;
    }
    else
    {
        wcerr << L"치명적 오류: 스킬 데이터 로드 실패. 게임을 시작할 수 없습니다." << endl;
    }

}

wstring CSkillDataManager::ConvertUtf8ToWString(const string& utf8_str)
{
    if (utf8_str.empty())
        return std::wstring();

    int required_size = MultiByteToWideChar(
        CP_UTF8,        // 입력 문자열 인코딩: UTF-8
        0,              // 플래그 (일반적으로 0)
        utf8_str.c_str(), // 입력 문자열
        (int)utf8_str.length(), // 입력 문자열 길이 (널 문자 제외)
        nullptr,        // 출력 버퍼 (nullptr이면 크기만 계산)
        0               // 출력 버퍼 크기 (0이면 크기만 계산)
    );

    if (required_size == 0)
    {
        std::cerr << "UTF-8 to wstring conversion error detected (MultiByteToWideChar failed to determine size)." << std::endl;
        return L"";
    }

    std::vector<wchar_t> wide_buffer(required_size + 1);

    int result = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8_str.c_str(),
        (int)utf8_str.length(),
        wide_buffer.data(), // 버퍼 포인터
        required_size       // 위에서 계산한 필요한 크기
    );

    if (result == 0)
    {
        std::cerr << "UTF-8 to wstring conversion error detected (MultiByteToWideChar failed to convert)." << std::endl;
        return L"";
    }

    return std::wstring(wide_buffer.data(), required_size);
}

bool CSkillDataManager::LoadData(const wstring& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        wcerr << L"Error: Failed to open JSON file: " << filename << endl;
        return false;
    }

    json j;
    file >> j; 

    if (j.contains("Skill_Data") && j["Skill_Data"].is_array())
    {
        for (const auto& skill_json : j["Skill_Data"])
        {
            FSkillData data;

            string name_utf8 = skill_json["Name"].get<string>();
            string desc_utf8 = skill_json["Desc"].get<string>();

            data.Name = ConvertUtf8ToWString(name_utf8);
            data.Desc = ConvertUtf8ToWString(desc_utf8);

            data.Damage = skill_json["Damage"].get<int>();

            m_SkillTable[data.Name] = data;
        }
        return true;
    }
    else
    {
        wcerr << L"Error: JSON file is missing the 'Skill_Data' array." << endl;
        return false;
    }
}

const FSkillData* CSkillDataManager::GetSkillData(const wstring& name) const
{
    auto it = m_SkillTable.find(name);
    if (it != m_SkillTable.end())
    {
        return &it->second; 
    }
    return nullptr; 
}