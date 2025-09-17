#include "stdafx.h"
#include "CResourceManager.h"

CResourceManager* CResourceManager::Instance = nullptr;



shared_ptr<ObImage> CResourceManager::CreateImage(const wstring& _path, const Vec2& _scale, bool _visible)
{
	auto safeDeleter = [](ObImage* img) { SafeDelete(img); };

	shared_ptr<ObImage> image(new ObImage(_path), safeDeleter);
	image->scale = _scale;
	image->maxFrame = Int2(1,1);
	image->frame = Int2(0, 0);
	image->visible = _visible;
	
	return image;
}

shared_ptr<ObImage> CResourceManager::CreateSprite(const wstring& _path, const Vec2& _scale, const Int2& _maxFrame, bool _visible)
{
	auto safeDeleter = [](ObImage* img){ SafeDelete(img);};

	shared_ptr<ObImage> image(new ObImage(_path), safeDeleter);
	image->scale = _scale;
	image->maxFrame = _maxFrame;
	image->frame = Int2(0, 0);
	image->visible = _visible;

	return image;
}

shared_ptr<ObImage> CResourceManager::CreateUI(const wstring& _path, const Vec2& _scale, bool _visible, COLLIDER _collider)
{
	auto safeDeleter = [](ObImage* img) { SafeDelete(img); };

	shared_ptr<ObImage> image(new ObImage(_path), safeDeleter);
	image->scale = _scale;
	image->maxFrame = Int2(1, 1);
	image->frame = Int2(0, 0);
	image->visible = _visible;
	image->collider = _collider;
	image->space = SPACE::SCREEN;

	return image;
}

void CResourceManager::AddImage(EPlayerState _state, EPlayerDir _dir, ObImage* _img)
{
	// TODO : 리소스 매니저에서 Sprites 관리할거면 추가
}

ObImage* CResourceManager::GetImage(EPlayerState _state, EPlayerDir _dir)
{

	return nullptr;
}
