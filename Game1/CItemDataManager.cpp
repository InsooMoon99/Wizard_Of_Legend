#include "stdafx.h"
#include "CItemDataManager.h"
#include "CResourceManager.h"

bool CItemDataManager::Init(const wstring& path)
{
    m_LastPath = path;
    return LoadFromFile(m_LastPath);
}

bool CItemDataManager::Reload()
{
    if (m_LastPath.empty()) return false;
    return LoadFromFile(m_LastPath);
}

void CItemDataManager::Clear()
{
    m_Table.clear();
    m_IconCache.clear();
}

const FItemData* CItemDataManager::GetById(const string& id) const
{
    auto it = m_Table.find(id);
    return (it == m_Table.end()) ? nullptr : &it->second;
}

shared_ptr<ObImage> CItemDataManager::GetIcon(const string& id)
{
    const FItemData* data = GetById(id);
    if (!data || data->IconKey.empty()) return nullptr;

    auto hit = m_IconCache.find(data->IconKey);
    if (hit != m_IconCache.end()) return hit->second;

    wstring keyW = Utf8ToW(data->IconKey);
    Vec2 size = Vec2((float)(data->IconW > 0 ? data->IconW : 40),
        (float)(data->IconH > 0 ? data->IconH : 40));

    auto img = RESOURCE->CreateUI(keyW, size, true);
    m_IconCache[data->IconKey] = img;
    return img;
}

vector<const FItemData*> CItemDataManager::GetAll() const
{
    vector<const FItemData*> out;
    out.reserve(m_Table.size());
    for (auto& kv : m_Table) out.push_back(&kv.second);
    return out;
}

bool CItemDataManager::LoadFromFile(const wstring& path)
{
    ifstream ifs(path, ios::binary);
    if (!ifs.is_open()) {
        wcerr << L"[ItemData] 파일 열기 실패: " << path << L"\n";
        return false;
    }
    json root;
    try { ifs >> root; }
    catch (...) {
        wcerr << L"[ItemData] JSON 파싱 실패(UTF-8 저장 확인)\n";
        return false;
    }
    return Parse(root);
}

bool CItemDataManager::Parse(const json& root)
{
    if (!root.contains("Item_Data") || !root["Item_Data"].is_array()) {
        wcerr << L"[ItemData] 'Item_Data' 배열 없음\n";
        return false;
    }

    m_Table.clear();
    m_IconCache.clear();

    for (auto& e : root["Item_Data"])
    {
        FItemData d{};
        d.Id = e.value("Id", "");
        d.Name = Utf8ToW(e.value("Name", ""));
        d.Desc = Utf8ToW(e.value("Desc", ""));
        d.Price = GetInt(e, "Price", 0);
        d.IconKey = e.value("IconKey", "");
        d.IconW = GetInt(e, "IconW", 0);
        d.IconH = GetInt(e, "IconH", 0);

        {
            string itemType = e.value("ItemType", "Etc");
            string statType = e.value("StatType", "None");
            string option = e.value("Option", "Add");

            ParseItemType(itemType, d.ItemType);
            ParseStatType(statType, d.ItemStatType);
            ParseOption(option, d.Option);
        }

        d.Value = GetFlt(e, "Value", 0.f);

        if (!d.Id.empty())
            m_Table.emplace(d.Id, move(d));
    }
    return true;
}

bool CItemDataManager::ParseItemType(const string& s, EItemType& out)
{
    if (s == "Consumable" || s == "Potion")     out = EItemType::Consumable;
    else if (s == "Weapon")                     out = EItemType::Weapon;
    else if (s == "Armor")                      out = EItemType::Armor;
    else                                        out = EItemType::Etc;
    return true;
}
bool CItemDataManager::ParseStatType(const string& s, EItem_StatType& out)
{
    if (s == "None")              out = EItem_StatType::None;
    else if (s == "Hp")           out = EItem_StatType::Hp;
    else if (s == "Attack")       out = EItem_StatType::Attack;
    else if (s == "Defense")      out = EItem_StatType::Defense;
    else                          out = EItem_StatType::None;
    return true;
}
bool CItemDataManager::ParseOption(const string& s, EModifyOption& out)
{
    if (s == "Add")               out = EModifyOption::Add;
    else if (s == "Multi")        out = EModifyOption::Multi;
    else if (s == "Set")          out = EModifyOption::Set;
    else                          out = EModifyOption::Add;
    return true;
}

wstring CItemDataManager::Utf8ToW(const string& s)
{
    if (s.empty()) return L"";
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    wstring w(n, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), w.data(), n);
    return w;
}
int CItemDataManager::GetInt(const json& obj, const char* key, int def)
{
    if (!obj.contains(key)) return def;
    const auto& v = obj.at(key);
    if (v.is_number_integer()) return v.get<int>();
    if (v.is_number()) return (int)v.get<double>();
    if (v.is_string()) { try { return stoi(v.get<string>()); } catch (...) {} }
    return def;
}
float CItemDataManager::GetFlt(const json& obj, const char* key, float def)
{
    if (!obj.contains(key)) return def;
    const auto& v = obj.at(key);
    if (v.is_number_float()) return (float)v.get<double>();
    if (v.is_number_integer()) return (float)v.get<int>();
    if (v.is_string()) { try { return stof(v.get<string>()); } catch (...) {} }
    return def;
}
