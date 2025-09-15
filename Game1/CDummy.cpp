#include "stdafx.h"
#include "CDummy.h"
#include "CResourceManager.h"
#include "CStatusComponent.h"

CDummy::CDummy()
{
	m_Status->SetHp(100.f);

	Resource_Settings();
}

CDummy::~CDummy()
{
	Release();
}

void CDummy::BeginPlay()
{
	CCharacter::BeginPlay();

}

void CDummy::Tick()
{
	CCharacter::Tick();

	m_DummyCol->Update();
	Dummy_Image->Update();
}

void CDummy::LateUpdate()
{
	CCharacter::LateUpdate();

}

void CDummy::Render()
{
	CCharacter::Render();

	m_DummyCol->Render();
	Dummy_Image->Render();
}

void CDummy::Release()
{
	CCharacter::Release();

	SafeDelete(m_DummyCol);
}

void CDummy::Resource_Settings()
{
	m_DummyCol = new ObRect();
	m_DummyCol->scale = Vec2(200.f, 200.f);
	m_DummyCol->visible = true;
	m_DummyCol->isFilled = false;
	m_DummyCol->collider = COLLIDER::RECT;

	Dummy_Image = RESOURCE->CreateImage(L"Object\\BossDummy.png", Vec2(29.f, 48.f), true);
	Dummy_Image->SetParentRT(*m_DummyCol);
	Dummy_Image->scale = Vec2(29.f, 48.f) * 3.0f;
	Dummy_Image->visible = true;
}
