#pragma once

class CResourceManager
{
private:
	CResourceManager()  = default;
	~CResourceManager() = default;

public:
	void		Init();
	void		Update();
	void		LateUpdate();
	void		Render();
	void		Release();

public:
	static CResourceManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CResourceManager();
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
	static CResourceManager* Instance;
};

