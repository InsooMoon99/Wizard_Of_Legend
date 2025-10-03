#include "stdafx.h"
#include "CSceneManager.h"

#include "CStage_Start.h"
#include "CStage01.h"
#include "CStageBoss.h"
#include "CStageFire.h"
#include "CStage_Edit.h"
#include "Main.h"

#include "CObjectManager.h"
#include "CCollisionManager.h"

CSceneManager* CSceneManager::Instance = nullptr;

void CSceneManager::Init()
{
}

void CSceneManager::Update()
{
    ApplyPendingChange();

    if (m_Scene) m_Scene->Update();
}

void CSceneManager::LateUpdate()
{
    if (m_Scene) m_Scene->LateUpdate();
}

void CSceneManager::Render()
{
    if (m_Scene) m_Scene->Render();
}

void CSceneManager::Release()
{
    if (m_Scene) m_Scene->Release();

    OBJECT->Release();        

    SafeDelete(m_Scene);
    m_SceneType = ESceneType::Stage_End;
    m_PendingChange = false;
    m_NextType = ESceneType::Stage_End;
}

void CSceneManager::Change_Scene(ESceneType _SceneType)
{
    if (m_Scene && m_SceneType == _SceneType) return;

    m_NextType = _SceneType;
    m_PendingChange = true;
}

void CSceneManager::ApplyPendingChange()
{
    if (!m_PendingChange) return;

    if (m_Scene) m_Scene->Release();

    SafeDelete(m_Scene);

    m_Scene = CreateScene(m_NextType);
    m_SceneType = m_NextType;

    m_PendingChange = false;
    m_NextType = ESceneType::Stage_End;

    if (m_Scene) m_Scene->Init();
}

CScene* CSceneManager::CreateScene(ESceneType type)
{
    switch (type)
    {
    case ESceneType::Start:     return new CStage_Start();
    case ESceneType::Stage01:   return new CStage01();
    case ESceneType::StageFire: return new CStageFire();
    case ESceneType::StageBoss: return new CStageBoss();
    case ESceneType::Edit:      return new CStage_Edit();
    case ESceneType::Stage_End: break;
    }
    return nullptr;
}
