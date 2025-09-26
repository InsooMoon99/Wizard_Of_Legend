// JsonRowTable.h
#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "json.hpp"

using std::string;
using std::wstring;
using std::unordered_map;
using std::ifstream;
using std::ios_base;
using std::size_t;
namespace fs = std::filesystem;

class JsonTable {
public:
    // 예: arrayNode="results", keyField="Vocabulary"
    bool Load(const wstring& path, const string& arrayNode, const string& keyField)
    {
        path_ = fs::path(path);
        arrayNode_ = arrayNode; keyField_ = keyField;
        return ReloadInternal();
    }

    void ReloadIfChanged() {
        if (path_.empty()) return;
        auto now = fs::last_write_time(path_);
        if (now != lastWrite_) ReloadInternal();
    }

    // 문자열/숫자/불린 필드 읽기
    string GetString(const string& key, const string& field, const string& fallback = "") const {
        const nlohmann::json* row = FindRow(key);
        if (!row || !row->contains(field) || !(*row)[field].is_string()) return fallback;
        return (*row)[field].get<string>();
    }

    double GetNumber(const string& key, const string& field, double fallback = 0.0) const {
        const nlohmann::json* row = FindRow(key);
        if (!row || !row->contains(field) || !(*row)[field].is_number()) return fallback;
        return (*row)[field].get<double>();
    }

    bool GetBool(const string& key, const string& field, bool fallback = false) const {
        const nlohmann::json* row = FindRow(key);
        if (!row || !row->contains(field) || !(*row)[field].is_boolean()) return fallback;
        return (*row)[field].get<bool>();
    }

    // 기존과 비슷하게 meaning만 바로 꺼내고 싶을 때
    string GetMeaning(const string& key, const string& fallback = "") const {
        return GetString(key, "meaning", fallback);
    }

    // 필요하면 원본 JSON 행 자체를 얻기
    const nlohmann::json* GetRaw(const string& key) const { return FindRow(key); }

    size_t Size() const { return rows_.size(); }

private:
    const nlohmann::json* FindRow(const string& key) const {
        auto it = rows_.find(key);
        return (it == rows_.end()) ? nullptr : &it->second;
    }

    bool ReloadInternal() {
        ifstream ifs(path_, ios_base::binary);
        if (!ifs) return false;

        nlohmann::json j;
        try { ifs >> j; }
        catch (...) { return false; }

        unordered_map<string, nlohmann::json> tmp;

        if (j.contains(arrayNode_) && j[arrayNode_].is_array()) {
            for (auto& row : j[arrayNode_]) {
                if (!row.contains(keyField_) || !row[keyField_].is_string()) continue;
                const string k = row[keyField_].get<string>();
                if (!k.empty()) tmp[k] = row; // 행 전체 저장!
            }
        }
        else {
            return false;
        }

        rows_.swap(tmp);
        lastWrite_ = fs::last_write_time(path_);
        return true;
    }

private:
    fs::path path_;
    fs::file_time_type lastWrite_{};
    string arrayNode_, keyField_;
    unordered_map<string, nlohmann::json> rows_; // key -> 행(JSON) 전체
};
