#include "stdafx.h"
#include "CUI_InventoryHUD.h"
#include "CResourceManager.h"
#include "CUI_InvenCloth.h"
#include "CUI_InvenSkill.h"
#include "CUI_InvenItem.h"
#include "CUI_InvenDesc.h"
#include "CUI_InvenStatus.h"
#include "CHelper.h"

CUI_InventoryHUD::CUI_InventoryHUD()
{
	Vec2 backgroundSize = Vec2(400.f, 700.f);
	m_Background = RESOURCE->CreateUI(L"UI\\Option.png", backgroundSize, true);
	m_Background->SetWorldPos(Vec2(-400.f, 0.f));
	m_Background->color = Color(0.3f, 0.3f, 0.3f, 0.45f);

	m_Cloth =  make_shared<CUI_InvenCloth>();
	m_Skill =  make_shared<CUI_InvenSkill>();
	m_Status = make_shared<CUI_InvenStatus>();
	m_Item  =  make_shared<CUI_InvenItem>();
	m_Desc  =  make_shared<CUI_InvenDesc>();

	AddChild(m_Cloth);
	AddChild(m_Skill);
	AddChild(m_Status);
	AddChild(m_Item);
	AddChild(m_Desc);

	bRender = false;
}

CUI_InventoryHUD::~CUI_InventoryHUD()
{
	Release();

}	

void CUI_InventoryHUD::BeginPlay()
{
	CUI_HUD::BeginPlay();

	FOnRendering.Add([this](bool bCheck)
		{
			SetRendering(bCheck);
		});

	m_Skill->FOnSkillIntersectMouse.Add([this](ESkillSlot index)
		{
			m_Desc->OnSkillMouseIntersect(index);
		});

	m_Item->FOnItemIntersectMouse.Add([this](int index)
		{
			m_Desc->OnItemMouseIntersect(index);
		});
}

void CUI_InventoryHUD::Tick()
{
	CUI_HUD::Tick();

	//#ifdef _DEBUG
	//if (ImGui::Begin("Debug"))
	//{
	//	ImGui::NewLine();
	//	Vec2 pos = m_Inventory[2]->GetWorldPos();
	//	ImGui::InputFloat2("Inventory : ", &pos.x);
	//	//m_Inventory[2]->SetWorldPos(pos);

	//}
	//
	//ImGui::End();
//#endif

	m_Background->Update();
}

void CUI_InventoryHUD::LateUpdate()
{
	CUI_HUD::LateUpdate();
}

void CUI_InventoryHUD::Render()
{
	if (!GetRender()) return;

	m_Background->Render();

	CUI_HUD::Render();
	
}

void CUI_InventoryHUD::Release()
{
	CUI_HUD::Release();

}

void CUI_InventoryHUD::SetRendering(bool _Render)
{
	CUI_HUD::SetRendering(_Render);

	for (auto& child : m_Children)
	{
		child->FOnRendering.Broadcast(_Render);
	}

}
