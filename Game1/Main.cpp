#include "stdafx.h"
#include "Main.h"

#include "CSceneManager.h"

void Main::Init()
{
	SCENE->Change_Scene(ESceneType::Stage01);
	ShowCursor(false);
}

void Main::Update()
{

	SCENE->Update();
}

void Main::LateUpdate()
{
	SCENE->LateUpdate();
}

void Main::Render()
{
	SCENE->Render();
	 
}

void Main::Release()
{
	SCENE->Release();

}

void Main::ResizeScreen()
{

}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    app.SetAppName(L"Game1");
    app.SetInstance(instance);
	app.InitWidthHeight(1200.f,800.f);
	Main* main = new Main();
	int wParam = (int)WIN->Run(main);
	WIN->DeleteSingleton();
	SafeDelete(main);
	return wParam;
}