#include "stdafx.h"
#include "CSceneManager.h"

#include "CStage_Edit.h"
#include "CStage01.h"
#include "CStageBoss.h"
#include "CStageFire.h"
#include "CStage_Start.h"
#include "Main.h"

CSceneManager* CSceneManager::Instance = nullptr;

void CSceneManager::Init()
{
	
}

void CSceneManager::Update()
{
	if (m_Scene)
		m_Scene->Update();
}

void CSceneManager::LateUpdate()
{
	if (m_Scene)
		m_Scene->LateUpdate();
}

void CSceneManager::Render()
{
	if (m_Scene)
		m_Scene->Render();
}

void CSceneManager::Release()
{
	if (m_Scene)
		m_Scene->Release();
}

void CSceneManager::Change_Scene(ESceneType _SceneType)
{
	if (m_SceneType == _SceneType)
		return;

	CScene* newScene = nullptr;

	switch (_SceneType)
	{
	case ESceneType::Start:
		newScene = new CStage_Start();
		m_SceneType = ESceneType::Start;
		break;

	case ESceneType::Stage01:
		newScene = new CStage01();
		m_SceneType = ESceneType::Stage01;
		break;

	case ESceneType::StageFire:
		newScene = new CStageFire();
		m_SceneType = ESceneType::StageFire;
		break;

	case ESceneType::StageBoss:
		newScene = new CStageBoss();
		m_SceneType = ESceneType::StageBoss;
		break;

	case ESceneType::Edit:
		newScene = new CStage_Edit();
		m_SceneType = ESceneType::Edit;
		break;

	case ESceneType::Stage_End: 
		break;
	}

	SafeDelete(m_Scene);

	m_Scene = newScene;
	m_SceneType = _SceneType;

	m_Scene->Init();
}
