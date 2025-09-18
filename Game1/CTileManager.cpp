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

void CTileManager::QueryTilesAABB(const Vec2& minW, const Vec2& maxW, vector<CTile*>& out)
{
	int minX = (int)floorf(minW.x / TILECX);
	int maxX = (int)floorf(maxW.x / TILECX);
	int minY = (int)floorf(minW.y / TILECY);
	int maxY = (int)floorf(maxW.y / TILECY);

	minX = max(minX, 0); minY = max(minY, 0);
	maxX = min(maxX, TILEX - 1); maxY = min(maxY, TILEY - 1);

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			int idx = y * TILEX + x;
			if ((size_t)idx >= m_vecTile.size())
				continue;

			auto* tile = dynamic_cast<CTile*>(m_vecTile[idx]);

			if (tile)
				out.push_back(tile);
		}
	}
		
}

bool CTileManager::IsBlockedAABB(const Vec2& minW, const Vec2& maxW)
{
	vector<CTile*> cand;
	QueryTilesAABB(minW, maxW, cand);

	for (auto* tile : cand)
	{
		if (tile->Get_DrawID() != (int)ETileType::TILE1) 
			continue; 
		// 간단 AABB 교차 체크
		Vec2 c = tile->Get_WorldPos();
		Vec2 h(TILECX * 0.5f, TILECY * 0.5f);
		Vec2 tMin = c - h, tMax = c + h;

		if (!(maxW.x <= tMin.x || minW.x >= tMax.x ||
			maxW.y <= tMin.y || minW.y >= tMax.y))
			return true; 
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
		int next = (cur == (int)ETileType::TILE1) ? (int)ETileType::TILE2 : (int)ETileType::TILE1;

		tile->Set_DrawID(next);
		tile->Set_Option(_Option);
	}
}

void CTileManager::Save_Data()
{

}

void CTileManager::Load_Data()
{

}
