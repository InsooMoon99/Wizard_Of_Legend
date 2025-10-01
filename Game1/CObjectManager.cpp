#include "stdafx.h"
#include "CObjectManager.h"

#include "CAttackActor.h"
#include "CBat_Dash_Shock.h"
#include "CSkill_Actor.h"
#include "CStar_Collision.h"

CObjectManager* CObjectManager::Instance = nullptr;

void CObjectManager::Init()
{

}

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
    }

    m_IsIterating = false;

    if (!m_PendingAdd.empty())
    {
        m_ActiveObjects.insert(m_ActiveObjects.end(), m_PendingAdd.begin(), m_PendingAdd.end());
        m_PendingAdd.clear();
    }
}

void CObjectManager::Render()
{
    for (auto& obj : m_ActiveObjects)
        obj->Render();
}

void CObjectManager::Release()
{
    m_PendingAdd.clear();
    m_ActiveObjects.clear();
}

shared_ptr<CActor> CObjectManager::SpawnActor(std::shared_ptr<CActor> actor)
{
    if (!actor) 
        return nullptr;

    actor->BeginPlay();

    if (m_IsIterating)
        m_PendingAdd.emplace_back(actor);

    else
        m_ActiveObjects.emplace_back(actor);

    return actor;
}

