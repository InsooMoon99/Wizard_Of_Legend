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
	m_vecTile.clear();

	// 타일 초기 생성 X
	//for (int i = 0; i < TILEY; ++i)
	//{
	//	for (int j = 0; j <  TILEX; ++j)
	//	{
	//		float fX = (float)(TILECX >> 1) + (TILECX * j);
	//		float fY = (float)(TILECY >> 1) + (TILECY * i);
	//
	//		CActor* tile = new CTile();
	//		tile->BeginPlay();
	//		tile->Get_Collision()->SetWorldPos(Vec2(fX, fY));
	//		tile->Set_WorldPos(Vec2(fX, fY));
	//
	//		m_vecTile.push_back(tile);
	//	}
	//}
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

	int minTileX = (int)floorf((camPos.x - halfW) / TILECX) - 1;
	int maxTileX = (int)floorf((camPos.x + halfW) / TILECX) + 1;
	int minTileY = (int)floorf((camPos.y - halfH) / TILECY) - 1;
	int maxTileY = (int)floorf((camPos.y + halfH) / TILECY) + 1;

	for (auto& pActor : m_vecTile)
	{
		if (auto* tile = dynamic_cast<CTile*>(pActor))
		{
			Vec2 tilePos = tile->Get_WorldPos();
			int x = (int)floorf(tilePos.x / TILECX);
			int y = (int)floorf(tilePos.y / TILECY);

			if (x >= minTileX && x <= maxTileX && y >= minTileY && y <= maxTileY)
			{
				tile->Render();
			}
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
	for (auto& pActor : m_vecTile)
	{
		auto* tile = dynamic_cast<CTile*>(pActor);

		if (tile)
		{
			Vec2 tilePos = tile->Get_WorldPos();
			Vec2 halfSize = tile->Get_Size() * 0.5f;

			Vec2 tileMin = tilePos - halfSize;
			Vec2 tileMax = tilePos + halfSize;

			if (!(_Max.x <= tileMin.x || _Min.x >= tileMax.x ||
				_Max.y <= tileMin.y || _Min.y >= tileMax.y))
			{
				out.push_back(tile);
			}
		}
	}
		
}

bool CTileManager::IsBlocked(const Vec2& _Min, const Vec2& _Max)
{
	vector<CTile*> collidableTiles;
	QueryTilesAABB(_Min, _Max, collidableTiles);

	for (auto* tile : collidableTiles)
	{
		if (tile->Get_DrawID() == (int)ETileType::WALL || tile->Get_DrawID() == (int)ETileType::FALL)
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

bool CTileManager::IsBlocked_ExceptFall(const Vec2& _Min, const Vec2& _Max, bool isDashing)
{
	vector<CTile*> collidableTiles;
	QueryTilesAABB(_Min, _Max, collidableTiles);

	for (auto* tile : collidableTiles)
	{
		if (isDashing && tile->Get_DrawID() == (int)ETileType::FALL)
			continue;

		if (tile->Get_DrawID() == (int)ETileType::WALL || tile->Get_DrawID() == (int)ETileType::FALL)
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

	CTile* foundTile = nullptr;
	for (auto& pActor : m_vecTile)
	{
		if (auto* tile = dynamic_cast<CTile*>(pActor))
		{
			Vec2 tilePos = tile->Get_WorldPos();
			if (floorf(tilePos.x / TILECX) == x && floorf(tilePos.y / TILECY) == y)
			{
				foundTile = tile;
				break;
			}
		}
	}

	if (foundTile)
	{
		int cur = foundTile->Get_DrawID();

		if (cur == (int)ETileType::WALL)
			cur = (int)ETileType::TILE2;

		else if (cur == (int)ETileType::TILE2)
			cur = (int)ETileType::FALL;

		else if (cur == (int)ETileType::FALL)
			cur = (int)ETileType::WALL;

		foundTile->Set_DrawID(cur);
		foundTile->Set_Option(_Option);
	}

	else
	{
		float fX = (float)(TILECX >> 1) + (TILECX * x);
		float fY = (float)(TILECY >> 1) + (TILECY * y);

		CActor* newTile = new CTile();
		newTile->BeginPlay();
		newTile->Get_Collision()->SetWorldPos(Vec2(fX, fY));
		newTile->Set_WorldPos(Vec2(fX, fY));

		dynamic_cast<CTile*>(newTile)->Set_DrawID(_DrawID);
		dynamic_cast<CTile*>(newTile)->Set_Option(_Option);

		m_vecTile.push_back(newTile);
	}		

}

void CTileManager::Picking(Vec2 _Pos, int _DrawID, int _Option, bool forceSet)
{
	CTile* foundTile = nullptr;

	for (auto& pActor : m_vecTile)
	{
		if (auto* tile = dynamic_cast<CTile*>(pActor))
		{
			Vec2 tilePos = tile->Get_WorldPos();
			Vec2 size = tile->Get_Size();

			float left = tilePos.x - size.x * 0.5f;
			float right = tilePos.x + size.x * 0.5f;
			float top = tilePos.y - size.y * 0.5f;
			float bottom = tilePos.y + size.y * 0.5f;

			if (_Pos.x >= left && _Pos.x < right &&
				_Pos.y >= top && _Pos.y < bottom)
			{
				foundTile = tile;
				break;
			}
		}
	}

	if (foundTile)
	{
		if (forceSet)
		{
			if (foundTile->Get_DrawID() != _DrawID)
				foundTile->Set_DrawID(_DrawID);

			foundTile->Set_Option(_Option);
			return;
		}

		int cur = foundTile->Get_DrawID();
		if (cur == (int)ETileType::WALL)       cur = (int)ETileType::TILE2;
		else if (cur == (int)ETileType::TILE2) cur = (int)ETileType::FALL;
		else if (cur == (int)ETileType::FALL)  cur = (int)ETileType::WALL;

		foundTile->Set_DrawID(cur);
		foundTile->Set_Option(_Option);
		return;
	}

	int cellX = (int)floorf(_Pos.x / TILECX);
	int cellY = (int)floorf(_Pos.y / TILECY);

	float fX = (float)(TILECX >> 1) + (TILECX * cellX);
	float fY = (float)(TILECY >> 1) + (TILECY * cellY);

	CTile* newTile = new CTile();
	newTile->BeginPlay();
	newTile->Get_Collision()->SetWorldPos(Vec2(fX, fY));
	newTile->Set_WorldPos(Vec2(fX, fY));

	newTile->Set_DrawID(_DrawID);
	newTile->Set_Option(_Option);

	m_vecTile.push_back(newTile);
}

void CTileManager::Save_Data()
{
	HANDLE	hFile = CreateFile(L"../Data/FireTile_First.dat", // 파일 경로(이름까지 명시)
		GENERIC_WRITE,		// 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,				// 공유 모드 설정 : 공유하지 않음
		NULL,				// 보안 속성 설정 : 기본 값 설정
		CREATE_ALWAYS,		// 생성 방식 : 쓰기 전용인 경우 파일이 없으면 생성하여 저장, OPEN_EXISTING : 읽기 전용 모드일 때 파일이 있을 경우에만 불러오기 실행)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성 : 일반 파일
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 주소(사용하지 않기 때문에 null)

	DWORD	dwByte(0);
	int iDrawID(0), iOption(0);
	Vec2 pos {};

	for (auto& pTile : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->Get_DrawID();
		iOption = dynamic_cast<CTile*>(pTile)->Get_Option();
		pos = dynamic_cast<CTile*>(pTile)->Get_WorldPos();

		WriteFile(hFile, &pos, sizeof(Vec2), &dwByte, nullptr);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	
}

void CTileManager::Load_Data(const wchar_t* _Path)
{
	if (!_Path) return;

	// 필요하면 기존 타일 비우기
	for (auto& p : m_vecTile) SafeDelete(p);
	m_vecTile.clear();

	HANDLE	hFile = CreateFile(_Path, // 파일 경로(이름까지 명시)
		GENERIC_READ,		// 파일 접근 모드(GENERIC_WRITE : 쓰기 전용, GENERIC_READ : 읽기 전용)
		NULL,				// 공유 모드 설정 : 공유하지 않음
		NULL,				// 보안 속성 설정 : 기본 값 설정
		OPEN_EXISTING,		// 생성 방식 : 쓰기 전용인 경우 파일이 없으면 생성하여 저장, OPEN_EXISTING : 읽기 전용 모드일 때 파일이 있을 경우에만 불러오기 실행)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성 : 일반 파일
		NULL); // 생성될 파일의 속성을 제공할 템플릿 파일 주소(사용하지 않기 때문에 null)

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD	dwByte(0);
	//INFO	tInfo{};
	int		iDrawID(0), iOption(0);
	Vec2	pos {};

	Release();

	while (true)
	{
		ReadFile(hFile, &pos, sizeof(Vec2), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CActor* pTile = new CTile();
		pTile->BeginPlay();
		pTile->Set_WorldPos(pos);

		// 위치 동기화
		if (auto* tile = dynamic_cast<CTile*>(pTile))
			if (auto col = tile->Get_Collision())
				col->SetWorldPos(pos);

		dynamic_cast<CTile*>(pTile)->Set_DrawID(iDrawID);
		dynamic_cast<CTile*>(pTile)->Set_Option(iOption);
		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

}

void CTileManager::ToggleTileImagesVisible()
{
	m_ShowTileImages = !m_ShowTileImages;

	for (auto* actor : m_vecTile)
	{
		if (auto* tile = dynamic_cast<CTile*>(actor))
		{
			tile->SetTileVisible(m_ShowTileImages);
		}
	}
}

void CTileManager::Erase_Tile()
{
	Vec2 mousePos = INPUT->GetMouseWorldPos();

	int x = (int)floorf(mousePos.x / TILECX);
	int y = (int)floorf(mousePos.y / TILECY);

	auto it = m_vecTile.begin();
	for (; it != m_vecTile.end(); ++it)
	{
		if (auto* tile = dynamic_cast<CTile*>(*it))
		{
			Vec2 tilePos = tile->Get_WorldPos();
			if (floorf(tilePos.x / TILECX) == x && floorf(tilePos.y / TILECY) == y)
			{
				break;
			}
		}
	}

	if (it != m_vecTile.end())
	{
		delete (*it);
		m_vecTile.erase(it);
	}
}
