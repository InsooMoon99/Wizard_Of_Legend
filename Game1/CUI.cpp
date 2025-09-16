#include "stdafx.h"
#include "CUI.h"

CUI::CUI()
{

}

CUI::~CUI()
{
	Release();
}

void CUI::BeginPlay()
{

}

void CUI::Tick()
{

}

void CUI::Render()
{

}

void CUI::Release()
{

}

ObRect* CUI::GetRect()
{
	ObRect* rect = new ObRect();
	rect->scale = m_Size;
	rect->SetWorldPos(m__Pos);

	return rect;
}

bool CUI::IsMouseInRect()
{
	ObRect* rect = GetRect();

	Vec2 mousePos = INPUT->GetMouseWorldPos();

	if (rect->IntersectScreenMouse(mousePos))
	{
		cout << "Test !" << endl;

		return true;
	}

	return false;
}
