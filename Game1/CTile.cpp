#include "stdafx.h"
#include "CTile.h"

CTile::CTile()
{
	//m_Collision = make_shared<ObRect>();
	//m_Collision->scale = Vec2(TILECX, TILECY);
	//m_Collision->visible = true;          
	//m_Collision->isFilled = false;
	//m_Collision->pivot = OFFSET_N;      
	//m_Collision->collider = COLLIDER::RECT;
	//m_Collision->Set_Owner(this);

	shared_ptr<ObImage> tile1 = RESOURCE->CreateImage(L"Edit\\WALL.bmp", Vec2(32.f, 32.f) * 2.f, true);
	tile1->collider = COLLIDER::RECT;
	//tile1->SetParentRT(*m_Collision);

	shared_ptr<ObImage> tile2 = RESOURCE->CreateImage(L"Edit\\FLOOR.bmp", Vec2(32.f, 32.f) * 2.f, true);
	tile2->collider = COLLIDER::RECT;
	//tile2->SetParentRT(*m_Collision);

	m_Tiles.insert({ ETileType::TILE1, tile1 });
	m_Tiles.insert({ ETileType::TILE2, tile2 });


}

CTile::~CTile()
{
	Release();
}

void CTile::BeginPlay()
{
	CActor::BeginPlay();

	m_Size = Vec2(TILECX, TILECY);

}

void CTile::Tick()
{
	CActor::Tick();

	//if (m_Collision)
	//	m_Collision->SetWorldPos(Get_WorldPos());

	//m_Collision->Update();

	if (m_DrawID < (int)ETileType::TILE1 || m_DrawID >= (int)ETileType::TILE_END)
	{
		m_TileType = ETileType::TILE1;
	}
	else
	{
		m_TileType = (ETileType)m_DrawID;
	}

	auto it = m_Tiles.find(m_TileType);
	if (it != m_Tiles.end())
	{
		it->second->SetWorldPos(Get_WorldPos());
		it->second->Update();
	}


}

void CTile::LateUpdate()
{
	CActor::LateUpdate();

}

void CTile::Render()
{
	CActor::Render();

	//m_Collision->Render();

	m_TileType = (ETileType)m_DrawID;
	
	auto it = m_Tiles.find(m_TileType);
	if (it != m_Tiles.end())
	{
		it->second->Render();
	}

}

void CTile::Release()
{
	CActor::Release();

	m_Tiles.clear();
}
