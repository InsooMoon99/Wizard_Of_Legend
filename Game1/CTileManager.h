#pragma once

class CActor;
class CTile;

class CTileManager
{
private:
	CTileManager();
	~CTileManager();

public:
	void	Init();
	void	Update();
	void	LateUpdate();
	void	Render();
	void	Release();

public:
	static CTileManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CTileManager();
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
	void	QueryTilesAABB(const Vec2& _Min, const Vec2& _Max, vector<CTile*>& out);
	bool	IsBlocked(const Vec2& _Min, const Vec2& _Max);
	bool	IsBlocked_ExceptFall(const Vec2& _Min, const Vec2& _Max, bool isDashing);

	void	Picking(Vec2 _Pos, int _DrawID, int _Option);
	void	Picking(Vec2 _Pos, int _DrawID, int _Option, bool forceSet);
	void	Erase_Tile();

	void	Save_Data();
	void	Load_Data(const wchar_t* _Path);

	// 이미지 렌더 처리
	void	ToggleTileImagesVisible();
	bool	IsTileImagesVisible() const { return m_ShowTileImages; }

	bool IsAllFall(const Vec2& minW, const Vec2& maxW) const;
	ETileType GetTileTypeAt(const Vec2& world) const;

private:
	static CTileManager*	Instance;

	vector<CActor*>	m_vecTile;
	bool m_ShowTileImages = false;

};

