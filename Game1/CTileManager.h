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
	void	QueryTilesAABB(const Vec2& minW, const Vec2& maxW, vector<CTile*>& out);
	bool	IsBlockedAABB(const Vec2& minW, const Vec2& maxW);

	void	Picking(Vec2 _Pos, int _DrawID, int _Option);

	void	Save_Data();
	void	Load_Data();



private:
	static CTileManager*	Instance;

private:
	vector<CActor*>	m_vecTile;
};

