#include "stdafx.h"
#include "CResourceManager.h"

CResourceManager* CResourceManager::Instance = nullptr;

void CResourceManager::Init()
{
	SoundSettings();
}

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

void CResourceManager::AddImage(ECharacterState _state, EPlayerDir _dir, ObImage* _img)
{
	// TODO : 리소스 매니저에서 Sprites 관리할거면 추가
}

ObImage* CResourceManager::GetImage(ECharacterState _state, EPlayerDir _dir)
{

	return nullptr;
}

void CResourceManager::SoundSettings()
{
	// Scene
	{
		SOUND->AddSound("Title.wav", "Title", true);
		SOUND->AddSound("Tutorial.wav", "Tutorial", true);
		SOUND->AddSound("Boss2.wav", "BossMap", true);
	}

	// Player
	{
		SOUND->AddSound("PlayerFootstep.wav", "PlayerStep", false);
		SOUND->AddSound("Dash.wav", "Dash", false);
	}

	// Skill
	{
		SOUND->AddSound("Fireball.wav", "FireBall");
		SOUND->AddSound("ImpactFire.wav", "FireBall_End");

		SOUND->AddSound("FireArcStart.wav", "FireArc");
		SOUND->AddSound("FireArcEnd.wav", "FireArc_End");

		SOUND->AddSound("WindSlashBasic.wav", "WindSlash");

		SOUND->AddSound("GAIA_ARMOR_START.mp3", "Earth_Shield");

	}

	// Hit
	{
		SOUND->AddSound("ImpactAir.wav", "ImpactAir");
	}

	// Boss
	{
		SOUND->AddSound("ImpactLightning.wav", "ImpactLightning");
		SOUND->AddSound("ImpactLightning.wav", "ImpactLightning2");
		SOUND->AddSound("MetalBreak.wav", "MetalBreak");
		SOUND->AddSound("MetalClang.wav", "MetalClang");

	}
}

