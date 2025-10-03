#include "stdafx.h"
#include "CRenderManager.h"

CRenderManager* CRenderManager::Instance = nullptr;

CRenderManager::CRenderManager()
{

}

CRenderManager::~CRenderManager()
{
	Release();
}

void CRenderManager::Init()
{

}

void CRenderManager::Render()
{
	for (int i = 0; i < static_cast<int>(ERenderLayer::Render_END); i++)
	{
		for (auto& iter : m_RenderQueue[i])
		{
			if (iter)
				iter->Render();
		}
	}
}

void CRenderManager::Release()
{
	for (int i = 0; i < static_cast<int>(ERenderLayer::Render_END); ++i)
	{
		m_RenderQueue[i].clear();
	}
}

void CRenderManager::AddRenderObject(CGameObject* obj)
{
	if (obj)
	{
		m_RenderQueue[static_cast<int>(obj->GetRenderLayer())].push_back(obj);
	}
}
