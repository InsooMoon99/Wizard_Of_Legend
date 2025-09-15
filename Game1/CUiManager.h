#pragma once
class CUIManager
{
private:
	CUIManager()  = default;
	~CUIManager() = default;

public:
	void		Init();
	void		Update();
	void		Render();
	void		Release();

public:
	static CUIManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CUIManager();
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
	static CUIManager* Instance;
};

