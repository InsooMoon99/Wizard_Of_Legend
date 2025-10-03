#include "stdafx.h"
#include "CObjectManager.h"
#include "CActor.h"

CObjectManager* CObjectManager::Instance = nullptr;

void CObjectManager::Init() {}

void CObjectManager::Update()
{
    m_IsIterating = true;

    for (auto it = m_ActiveObjects.begin(); it != m_ActiveObjects.end(); )
    {
        auto obj = *it;
        obj->Tick();

        if (obj->IsDestroy())
            it = m_ActiveObjects.erase(it);  
        else
            ++it;

        if (m_PendingClear) break;
    }

    m_IsIterating = false;

    if (m_PendingClear)
    {
        ForceClear();
        return; 
    }

    FlushPendingAdds();
}

void CObjectManager::FlushPendingAdds()
{
    if (m_PendingAdd.empty()) return;

    for (auto& a : m_PendingAdd) {
        m_ActiveObjects.emplace_back(a);
        a->BeginPlay();             
    }
    m_PendingAdd.clear();
}

void CObjectManager::Render()
{
    for (auto& obj : m_ActiveObjects)
        obj->Render();
}

void CObjectManager::ForceClear()
{
    m_PendingAdd.clear();
    m_ActiveObjects.clear(); 
    m_PendingClear = false;
}

void CObjectManager::Release()
{
    if (m_IsIterating) 
    {
        m_PendingClear = true; 
        return;
    }
    ForceClear(); 
}

void CObjectManager::RequestClear()
{
    if (m_IsIterating) 
    {
        m_PendingClear = true; 
        return;
    }

    ForceClear();              
}

shared_ptr<CActor> CObjectManager::SpawnActor(shared_ptr<CActor> actor)
{
    if (!actor) return nullptr;

    if (m_IsIterating || m_PendingClear) 
    {
        m_PendingAdd.emplace_back(actor);
        return actor;
    }

    m_ActiveObjects.emplace_back(actor);
    actor->BeginPlay();
    return actor;
}
