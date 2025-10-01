#include "stdafx.h"
#include "CItem.h"

CItem::CItem()
{
	
}

CItem::~CItem()
{
	Release();
}

void CItem::BeginPlay()
{
	CActor::BeginPlay();


}

void CItem::Tick()
{
	CActor::Tick();

	if (m_PendingKill) return;

	if (m_ItemImg)
		m_ItemImg->Update();


}

void CItem::Render()
{
	CActor::Render();

	if (m_PendingKill) return;

	if (m_ItemImg)
		m_ItemImg->Render();
}

void CItem::LateUpdate()
{
	CActor::LateUpdate();

}

void CItem::Release()
{
	CActor::Release();


}

void CItem::SetData(const FItemData& data)
{
	m_ItemData = data;

	ApplyData();
}

void CItem::ApplyData()
{
	m_ItemImg = ITEM_DATA->GetIcon(m_ItemData.Id);

	if (m_ItemImg)
	{
		m_Size = Vec2(m_ItemData.IconW, m_ItemData.IconH);

		m_ItemImg->visible = true;
	}
}

void CItem::DisableAndDestroy()
{
	if(m_Collision)
	{
		COLLISION->RemoveCollider(m_Collision.get());
		m_Collision.reset();          
	}

	if (m_ItemImg)
		m_ItemImg->visible = false;    

	m_vecCol.clear();                  
	m_PendingKill = true;           
}
