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

		SOUND->AddSound("SELECT_MENU.mp3", "Select", false);
	}

	// Player
	{
		SOUND->AddSound("PlayerFootstep.wav", "PlayerStep", false);
		SOUND->AddSound("Dash.wav", "Dash", false);
		SOUND->AddSound("RunSpeed.wav", "RunSpeed", false);
	}

	// Skill
	{
		SOUND->AddSound("Fireball.wav", "FireBall");
		SOUND->AddSound("ImpactFire.wav", "FireBall_End");

		SOUND->AddSound("FireArcStart.wav", "FireArc");
		SOUND->AddSound("FireArcEnd.wav", "FireArc_End");

		SOUND->AddSound("WindSlashBasic.wav", "WindSlash");

		SOUND->AddSound("GAIA_ARMOR_START.mp3", "Earth_Shield");

		SOUND->AddSound("ULT_USE.mp3", "EnhanceCharge");
	}

	// Hit
	{
		SOUND->AddSound("ImpactEarth.wav", "ImpactEarth");
		SOUND->AddSound("ImpactAir.wav", "ImpactAir");
		SOUND->AddSound("PLAYER_HITED_1.mp3", "PlayerHit");
	}
	// Meteor
	{
		SOUND->AddSound("GhoulGrowl.wav", "Ghoul");
		SOUND->AddSound("MeteorExplosion.wav", "MeteorEx");
		SOUND->AddSound("Teleport.wav", "Teleport");
	}

	// Blob
	{
		SOUND->AddSound("BlobAttack.wav", "BlobAttack");
		SOUND->AddSound("BlobCharge.wav", "BlobCharge");
		SOUND->AddSound("BlobDead.wav", "BlobDead");
		SOUND->AddSound("BlobHurt.wav", "BlobHurt");
	}

	// Golem
	{
		SOUND->AddSound("golem_atk.mp3", "GAtk");
		SOUND->AddSound("golem_die.mp3", "GDie");
		SOUND->AddSound("golem_hit.mp3", "GHit");
	}

	// Wizard
	{
		SOUND->AddSound("OldWizard1.wav", "WAtk");
		SOUND->AddSound("EnemyDead6.wav", "WDead");
	}

	// Knight
	{
		SOUND->AddSound("knight_walkL.mp3", "KWalk");
		SOUND->AddSound("knight_atk0.mp3", "KAtk");
		SOUND->AddSound("knight_die.mp3", "Kdie");
		SOUND->AddSound("knight_hit1.mp3", "Khit");
	}

	// Drop
	{
		SOUND->AddSound("DropSpell.wav", "DropSpell");
		SOUND->AddSound("GoldSpawn.wav", "GoldSpawn");

		SOUND->AddSound("MenuBuy.wav", "BuyItem");
	}

	// Inventory
	{
		SOUND->AddSound("inventoryON.mp3", "InvenOn");
		SOUND->AddSound("inventoryOFF.mp3", "InvenOff");
	}

	// Boss
	{
		SOUND->AddSound("ImpactLightning.wav", "ImpactLightning");
		SOUND->AddSound("ImpactLightning.wav", "ImpactLightning2");
		SOUND->AddSound("MetalBreak.wav", "MetalBreak");
		SOUND->AddSound("MetalClang.wav", "MetalClang");

		SOUND->AddSound("LightningBossHurt0.wav", "BHurt1");
		SOUND->AddSound("LightningBossHurt1.wav", "BHurt2");
		SOUND->AddSound("LightningBossHurt2.wav", "BHurt3");
		SOUND->AddSound("LightningBossHurt3.wav", "BHurt4");

	}
}

