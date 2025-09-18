#include "stdafx.h"
#include "CTileManager.h"

#include "CTile.h"

CTileManager* CTileManager::Instance = nullptr;

CTileManager::CTileManager()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileManager::~CTileManager()
{
	Release();
}

void CTileManager::Init()
{
	m_vecTile.reserve(TILEX * TILEY);

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j <  TILEX; ++j)
		{
			float fX = (float)(TILECX >> 1) + (TILECX * j);
			float fY = (float)(TILECY >> 1) + (TILECY * i);

			CActor* tile = new CTile();
			tile->BeginPlay();
			tile->Get_Collision()->SetWorldPos(Vec2(fX, fY));
			tile->Set_WorldPos(Vec2(fX, fY));

			m_vecTile.push_back(tile);
		}
	}
}

void CTileManager::Update()
{
	for (auto& tile : m_vecTile)
		tile->Tick();
}

void CTileManager::LateUpdate()
{
	for (auto& tile : m_vecTile)
		tile->LateUpdate();
}

void CTileManager::Render()
{
	Vector2 camPos = CAM->GetCameraPos(); 
	const float halfW = WINCX * 0.5f;
	const float halfH = WINCY * 0.5f;

	// 최소 여유 일단 1칸
	int minTileX = (int)floorf((camPos.x - halfW) / TILECX) - 1;
	int maxTileX = (int)floorf((camPos.x + halfW) / TILECX) + 1;
	int minTileY = (int)floorf((camPos.y - halfH) / TILECY) - 1;
	int maxTileY = (int)floorf((camPos.y + halfH) / TILECY) + 1;

	minTileX = max(0, minTileX);
	minTileY = max(0, minTileY);
	maxTileX = min(TILEX - 1, maxTileX);
	maxTileY = min(TILEY - 1, maxTileY);

	for (int y = minTileY; y <= maxTileY; ++y)
	{
		for (int x = minTileX; x <= maxTileX; ++x)
		{
			const int idx = y * TILEX + x;

			if ((size_t)idx >= m_vecTile.size())
				continue;

			CActor* tile = m_vecTile[idx];
			if (!tile) continue;

			tile->Render();
		}
	}
}


void CTileManager::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), [](auto& p)
	{
		if (p)
		{
			delete p;
			p = nullptr;
		}
	});
	m_vecTile.clear();
}

void CTileManager::QueryTilesAABB(const Vec2& _Min, const Vec2& _Max, vector<CTile*>& out)
{
	int minX = (int)floorf(_Min.x / TILECX);
	int maxX = (int)floorf(_Max.x / TILECX);
	int minY = (int)floorf(_Min.y / TILECY);
	int maxY = (int)floorf(_Max.y / TILECY);

	minX = max(minX, 0);
	minY = max(minY, 0);
	maxX = min(maxX, TILEX - 1);
	maxY = min(maxY, TILEY - 1);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			int iIndex = y * TILEX + x;
			if ((size_t)iIndex >= m_vecTile.size())
				continue;

			auto* tile = dynamic_cast<CTile*>(m_vecTile[iIndex]);

			if (tile)
				out.push_back(tile);
		}
	}
		
}

bool CTileManager::IsBlocked(const Vec2& _Min, const Vec2& _Max)
{
	vector<CTile*> canDir;
	QueryTilesAABB(_Min, _Max, canDir);

	for (auto* tile : canDir)
	{
		bool bWallCheck = tile->Get_DrawID() != (int)ETileType::WALL;
		bool bFallCheck = tile->Get_DrawID() != (int)ETileType::FALL;

		if (bWallCheck && bFallCheck) continue;

		Vec2 tilePos = tile->Get_WorldPos();
		Vec2 height(TILECX * 0.5f, TILECY * 0.5f);
		Vec2 tileMin = tilePos - height, tileMax = tilePos + height;

		if (!(_Max.x <= tileMin.x || _Min.x >= tileMax.x ||
			_Max.y <= tileMin.y || _Min.y >= tileMax.y))
			return true; 
	}
	return false;
}

bool CTileManager::IsBlocked_ExceptFall(const Vec2& _Min, const Vec2& _Max, bool isDashing)
{
	vector<CTile*> canDir;
	QueryTilesAABB(_Min, _Max, canDir);

	for (auto* tile : canDir)
	{
		if (isDashing && tile->Get_DrawID() == (int)ETileType::FALL)
			continue;

		bool bWallCheck = tile->Get_DrawID() == (int)ETileType::WALL;
		bool bFallCheck = tile->Get_DrawID() == (int)ETileType::FALL;

		if (bWallCheck || bFallCheck)
		{
			Vec2 tilePos = tile->Get_WorldPos();
			Vec2 height(TILECX * 0.5f, TILECY * 0.5f);
			Vec2 tileMin = tilePos - height, tileMax = tilePos + height;

			if (!(_Max.x <= tileMin.x || _Min.x >= tileMax.x || _Max.y <= tileMin.y || _Min.y >= tileMax.y))
				return true;
		}
	}

	return false;
}

void CTileManager::Picking(Vec2 _Pos, int _DrawID, int _Option)
{
	Vec2 worldPos = _Pos;

	int x = (int)floorf(worldPos.x / TILECX);
	int y = (int)floorf(worldPos.y / TILECY);

	if (x < 0 || x >= TILEX || y < 0 || y >= TILEY) return;

	int index = y * TILEX + x;
	if ((size_t)index >= m_vecTile.size()) return;

	if (auto* tile = dynamic_cast<CTile*>(m_vecTile[index]))
	{
		int cur = tile->Get_DrawID();

		if (cur == (int)ETileType::WALL)
		{
			cur = (int)ETileType::TILE2;
		}

		else if (cur == (int)ETileType::TILE2)
		{
			cur = (int)ETileType::FALL;
		}

		else if (cur == (int)ETileType::FALL)
		{
			cur = (int)ETileType::WALL;
		}

		tile->Set_DrawID(cur);
		tile->Set_Option(_Option);
	}
}

void CTileManager::Save_Data()
{

}

void CTileManager::Load_Data()
{

}
