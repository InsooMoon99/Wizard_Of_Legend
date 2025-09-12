#pragma once

class CImGuiManager
{
private:
	CImGuiManager()  = default;
	~CImGuiManager() = default;

public:
	static CImGuiManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CImGuiManager();
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
	static CImGuiManager* Instance;
};

