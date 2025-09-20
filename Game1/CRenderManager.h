#pragma once

class CGameObject;

class CRenderManager
{
private:
	CRenderManager();
	~CRenderManager();

public:
	void		Init();
	void		Render();
	void		Release();

public:
	static CRenderManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CRenderManager();
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

public:
	void AddRenderObject(CGameObject* obj);

private:
	static CRenderManager* Instance;

	vector<CGameObject*> m_RenderQueue[static_cast<int>(ERenderLayer::Render_END)];

};

