#pragma once
class CUiManager
{
private:
	CUiManager()  = default;
	~CUiManager() = default;

public:
	void		Init();
	void		Update();
	void		Render();
	void		Release();

public:
	static CUiManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CUiManager();
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
	static CUiManager* Instance;
};

