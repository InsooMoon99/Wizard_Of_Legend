#include "stdafx.h"
#include "Main.h"
#include "CSceneManager.h"
#include "CCollisionManager.h"
#include "CParticleManager.h"
#include "CRenderManager.h"

void Main::Init()
{
	RESOURCE->Init();

	SCENE->Change_Scene(ESceneType::Stage02);
	SKILL->Init();
	PARTICLE->Init();
	TILE->Init();

}

void Main::Update()
{
	SCENE->Update();
	COLLISION->Update();
	SKILL->Update();
	PARTICLE->Update();
	TILE->Update();

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

	//PARTICLE->Render();

	if (ImGui::Begin("Debug"))
	{
		ImGui::Text("FPS : %d", TIMER->GetFramePerSecond());
		ImGui::NewLine();

		Vec2 pos = INPUT->GetMouseScreenPos();
		ImGui::Text("Mouse Local : x %f, y %f", pos.x, pos.y);

		Vec2 worldpos = INPUT->GetMouseWorldPos();
		ImGui::Text("Mouse World : x %f, y %f", worldpos.x, worldpos.y);
	}

	ImGui::End();
}

void Main::Release()
{
	SCENE->Destroy_Instance();
	TILE->Destroy_Instance();
	COLLISION->Destroy_Instance();
	PARTICLE->Destroy_Instance();
	RENDER->Destroy_Instance();
	SKILL->Destroy_Instance();

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