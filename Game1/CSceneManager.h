#pragma once
#include "CEnums.h"

class CScene;

class CSceneManager
{
private:
	CSceneManager()  = default;
	~CSceneManager() = default;

public:
	void		Init();
	void		Update();
	void		LateUpdate();
	void		Render();
	void		Release();

	void		Change_Scene(SceneType _SceneType);


public:
	static CSceneManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CSceneManager();
		}

		return Instance;
	}

	static void Destroy_Instance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static CSceneManager* Instance;

	CScene*	  m_Scene;
	SceneType m_SceneType = SceneType::Stage_End;
};

