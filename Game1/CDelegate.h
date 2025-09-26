#pragma once
#include <functional>
#include <vector>
#include <cstddef>
#include <algorithm>

template<typename... Args>
class CDelegate
{
public:
    using FuncType = function<void(Args...)>;
    using Token = size_t;

    Token Add(const FuncType& f)
	{
        const Token id = m_NextId++;
        m_Functions.push_back({ id, f });
        return id;
    }

    bool Remove(Token id)
	{
        if (m_IsBroadcasting) 
        {
            m_PendingRemovals.push_back(id);
            return true;
        }
        auto it = std::find_if(m_Functions.begin(), m_Functions.end(),
            [id](const Entry& e) { return e.id == id; });
        if (it != m_Functions.end()) {
            m_Functions.erase(it);
            return true;
        }
        return false;
    }

    void Clear() {
        if (m_IsBroadcasting) {
            for (const auto& e : m_Functions) m_PendingRemovals.push_back(e.id);
            return;
        }
        m_Functions.clear();
    }

    void Broadcast(Args... args)
	{
        m_IsBroadcasting = true;

        const size_t sz = m_Functions.size();
        for (size_t i = 0; i < sz; ++i) 
        {
            const auto& e = m_Functions[i];
            if (e.func) e.func(args...);
        }

        m_IsBroadcasting = false;

        if (!m_PendingRemovals.empty()) 
        {
            for (Token id : m_PendingRemovals) 
            {
                auto it = find_if(m_Functions.begin(), m_Functions.end(),
                    [id](const Entry& e) { return e.id == id; });
                if (it != m_Functions.end()) m_Functions.erase(it);
            }
            m_PendingRemovals.clear();
        }
    }

private:
    struct Entry
	{
        Token    id;
        FuncType func;
    };

    vector<Entry> m_Functions;
    Token m_NextId = 1;

    bool m_IsBroadcasting = false;
    vector<Token> m_PendingRemovals;
};
