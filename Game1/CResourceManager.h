#pragma once

class CResourceManager
{
private:
	CResourceManager()  = default;
	~CResourceManager() = default;

public:
	void Init();

public:
	shared_ptr<ObImage> CreateImage(
		const wstring& _path,
		const Vec2& _scale,
		bool _visible = false
	);

	shared_ptr<ObImage> CreateSprite(
		const wstring& _path, 
		const Vec2& _scale, 
		const Int2& _maxFrame, 
		bool _visible = false);

	shared_ptr<ObImage> CreateUI(
		const wstring& _path,
		const Vec2& _scale,
		bool _visible = false,
		COLLIDER _collider = COLLIDER::NONE
	);

	void     AddImage(ECharacterState _state, EPlayerDir _dir, ObImage* _img);
	ObImage* GetImage(ECharacterState _state, EPlayerDir _dir);

	void SoundSettings();

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

