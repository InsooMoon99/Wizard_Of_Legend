#pragma once
#include "CEnums.h"

class CScene;

class CSceneManager
{
private:
    CSceneManager() = default;
    ~CSceneManager() = default;

public:
    void        Init();
    void        Update();
    void        LateUpdate();
    void        Render();
    void        Release();

    void        Change_Scene(ESceneType _SceneType);
    ESceneType  Get_CurScene() { return m_SceneType; }

public:
    static CSceneManager* Get_Instance()
    {
        if (Instance == nullptr) Instance = new CSceneManager();
        return Instance;
    }
    static void Destroy_Instance()
    {
        if (Instance) { delete Instance; Instance = nullptr; }
    }

private:
    static CSceneManager* Instance;

    CScene* m_Scene = nullptr;
    ESceneType  m_SceneType = ESceneType::Stage_End;

    bool        m_PendingChange = false;
    ESceneType  m_NextType = ESceneType::Stage_End;

private:
    void    ApplyPendingChange();  
    CScene* CreateScene(ESceneType);
};
