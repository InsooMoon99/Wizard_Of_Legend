#include "stdafx.h"
#include "CResourceManager.h"

CResourceManager* CResourceManager::Instance = nullptr;



ObImage* CResourceManager::CreateImage(const wstring& _path, const Vec2& _scale, bool _visible)
{
	ObImage* image = new ObImage(_path);
	image->scale = _scale;
	image->maxFrame = Int2(1,1);
	image->frame = Int2(0, 0);
	image->visible = _visible;
	
	return image;
}

ObImage* CResourceManager::CreateSprite(const wstring& _path, const Vec2& _scale, const Int2& _maxFrame, bool _visible)
{
	ObImage* image = new ObImage(_path);
	image->scale = _scale;
	image->maxFrame = _maxFrame;
	image->frame = Int2(0, 0);
	image->visible = _visible;

	return image;
}

void CResourceManager::AddImage(PlayerState _state, PlayerDir _dir, ObImage* _img)
{
	// TODO : 리소스 매니저에서 Sprites 관리할거면 추가
}

ObImage* CResourceManager::GetImage(PlayerState _state, PlayerDir _dir)
{

	return nullptr;
}
