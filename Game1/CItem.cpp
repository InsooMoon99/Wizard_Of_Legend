#include "stdafx.h"
#include "CItem.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::BeginPlay()
{

}

void CItem::Tick()
{

}

void CItem::Render()
{

}

void CItem::LateUpdate()
{
	CActor::LateUpdate();

}

void CItem::Release()
{
	CActor::Release();

}

void CItem::OnComponentBeginOverlap(GameObject* collider, GameObject* other)
{
	CActor::OnComponentBeginOverlap(collider, other);

}

void CItem::OnComponentEndOverlap(GameObject* collider, GameObject* other)
{
	CActor::OnComponentEndOverlap(collider, other);

}

