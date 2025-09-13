#pragma once

class CResourceManager
{
private:
	CResourceManager()  = default;
	~CResourceManager() = default;

public:


public:
	ObImage* CreateImage(
		const wstring& _path,
		const Vec2& _scale,
		bool _visible = false
	);

	ObImage* CreateSprite(
		const wstring& _path, 
		const Vec2& _scale, 
		const Int2& _maxFrame, 
		bool _visible = false);

	void     AddImage(PlayerState _state, PlayerDir _dir, ObImage* _img);
	ObImage* GetImage(PlayerState _state, PlayerDir _dir);

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

