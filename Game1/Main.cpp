#include "stdafx.h"
#include "Main.h"
#include "CSceneManager.h"
#include "CCollisionManager.h"
#include "CParticleManager.h"
#include "CRenderManager.h"
#include "CSkillDataManager.h"

void Main::Init()
{
	// cmd에서 UTF-8 출력
	//SetConsoleOutputCP(CP_UTF8);

	RESOURCE->Init();

	SKILL_DATA->Init();
	ITEM_DATA->Init();
	OBJECT->Init();
	TILE->Init();
	PARTICLE->Init();
	SCENE->Change_Scene(ESceneType::StageBoss);
	SKILL->Init();
}

void Main::Update()
{

	RENDER->Release();

	PARTICLE->Update();
	TILE->Update();
	SCENE->Update();
	COLLISION->Update();
	SKILL->Update();
	OBJECT->Update();
}

void Main::LateUpdate()
{
	if (INPUT->KeyDown(VK_ESCAPE))
	{
		if (SCENE->Get_CurScene() != ESceneType::Start)
		{
			SCENE->Change_Scene(ESceneType::Start);
		}
	}

	SCENE->LateUpdate();
	TILE->LateUpdate();

}

void Main::Render()
{
	SCENE->Render();
	SKILL->Render();
	OBJECT->Render();


	//PARTICLE->Render();

	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("FPS : %d", TIMER->GetFPS());
		ImGui::NewLine();

		Vec2 pos = INPUT->GetMouseScreenPos();
		ImGui::Text("Mouse Local : x %f, y %f", pos.x, pos.y);

		Vec2 worldpos = INPUT->GetMouseWorldPos();
		ImGui::Text("Mouse World : x %f, y %f", worldpos.x, worldpos.y);
	}

	ImGui::End();

	RENDER->Render();
	PARTICLE->Render();

}

void Main::Release()
{
	SCENE->Destroy_Instance();
	TILE->Destroy_Instance();
	COLLISION->Destroy_Instance();
	RENDER->Destroy_Instance();
	PARTICLE->Destroy_Instance();
	SKILL->Destroy_Instance();
	OBJECT->Release();
}

void Main::ResizeScreen()
{

}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    app.SetAppName(L"Game1");
    app.SetInstance(instance);
	app.InitWidthHeight(WINCX,WINCY);
	Main* main = new Main();
	int wParam = (int)WIN->Run(main);
	WIN->DeleteSingleton();
	SafeDelete(main);
	return wParam;
}