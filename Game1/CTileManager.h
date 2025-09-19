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

	void	Save_Data();
	void	Load_Data();

	void	Erase_Tile();

private:
	static CTileManager*	Instance;

private:
	vector<CActor*>	m_vecTile;
};

