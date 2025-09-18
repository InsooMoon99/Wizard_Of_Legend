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

	Set_WorldPos(m_Collision->GetWorldPos());

	m_Collision->Update();
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
		ImGui::Text("Dummy Pos x : %f, y : %f", m_Collision->GetWorldPos().x, m_Collision->GetWorldPos().y);
	}

	ImGui::End();

	m_Collision->Render();
	Dummy_Image->Render();
}

void CDummy::Release()
{
	CCharacter::Release();

}

void CDummy::Resource_Settings()
{
	m_Collision = make_shared<ObRect>();
	m_Collision->scale = Vec2(100.f, 150.f);
	m_Collision->visible = true;
	m_Collision->isFilled = false;
	m_Collision->collider = COLLIDER::RECT;
	m_Collision->Set_Owner(this);
	COLLISION->AddCollider(m_Collision.get());

	Dummy_Image = RESOURCE->CreateImage(L"Object\\BossDummy.png", Vec2(29.f, 48.f), true);
	Dummy_Image->SetParentRT(*m_Collision);
	Dummy_Image->scale = Vec2(29.f, 48.f) * 3.0f;
	Dummy_Image->visible = true;
}

void CDummy::SetSprite(EPlayerState _State, EPlayerDir _Dir)
{
	CCharacter::SetSprite(_State, _Dir);


}
