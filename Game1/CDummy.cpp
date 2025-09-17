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

	if (ImGui::Begin("Dummy"))
	{
		ImGui::Text("Hp : %f", m_Status->GetHp());
	}

	ImGui::End();

	m_DummyCol->Render();
	Dummy_Image->Render();
}

void CDummy::Release()
{
	CCharacter::Release();

}

void CDummy::Resource_Settings()
{
	m_DummyCol = make_shared<ObRect>();
	m_DummyCol->scale = Vec2(100.f, 150.f);
	m_DummyCol->visible = true;
	m_DummyCol->isFilled = false;
	m_DummyCol->collider = COLLIDER::RECT;
	m_DummyCol->Set_Owner(this);
	COLLISION->AddCollider(m_DummyCol.get());

	Dummy_Image = RESOURCE->CreateImage(L"Object\\BossDummy.png", Vec2(29.f, 48.f), true);
	Dummy_Image->SetParentRT(*m_DummyCol);
	Dummy_Image->scale = Vec2(29.f, 48.f) * 3.0f;
	Dummy_Image->visible = true;
}
