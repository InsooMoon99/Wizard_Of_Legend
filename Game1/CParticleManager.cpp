#include "stdafx.h"
#include "CParticleManager.h"
#include "CResourceManager.h"

CParticleManager* CParticleManager::Instance = nullptr;

void CParticleManager::Init()
{
	Resource_Settings();
	Boss_Resource_Settings();
}

void CParticleManager::Update()
{
	for (auto it = m_ActiveParticles.begin(); it != m_ActiveParticles.end();)
	{
		auto particle = *it;
		particle->Update();

		// 스프라이트 재생이 끝난 파티클 제거
		if (particle->Get_AnimState() == ANIMSTATE::STOP)
		{
			it = m_ActiveParticles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CParticleManager::Render()
{
	for (auto& particle : m_ActiveParticles)
	{
		particle->Render();
	}
}

void CParticleManager::Release()
{
	m_Particles.clear();
}

void CParticleManager::Resource_Settings()
{
	shared_ptr<ObImage> hit_Effect = RESOURCE->CreateSprite(L"Particle\\HitEffect.png", Vec2(840.f / 4.f, 200.f), Int2(4, 1), true);
	hit_Effect->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

	shared_ptr<ObImage> hit_Spark = RESOURCE->CreateSprite(L"Particle\\HitSpark.png", Vec2(400.f / 8.f, 50.f) * 3.0f, Int2(8, 1), true);
	hit_Spark->ChangeAnim(ANIMSTATE::ONCE, 0.05f);

	shared_ptr<ObImage> hit_Spark_Red = RESOURCE->CreateSprite(L"Particle\\HitSpark.png", Vec2(400.f / 8.f, 50.f) * 3.0f, Int2(8, 1), true);
	hit_Spark_Red->color = Color(1.0f, 0.f, 0.f, 0.5f);
	hit_Spark_Red->ChangeAnim(ANIMSTATE::ONCE, 0.05f);

	shared_ptr<ObImage> fire_hitEffect = RESOURCE->CreateSprite(L"Particle\\Fire_HitEffect.png", Vec2(910.f / 7.f, 110.f) * 1.5f, Int2(7, 1), true);
	fire_hitEffect->ChangeAnim(ANIMSTATE::ONCE, 0.07f);

	shared_ptr<ObImage> fire_hitEffect2 = RESOURCE->CreateSprite(L"Particle\\Fire_HiitEffect2.bmp", Vec2(670.f / 5.f, 175.f) * 0.8f, Int2(5, 1), true);
	fire_hitEffect2->ChangeAnim(ANIMSTATE::ONCE, 0.07f);

	shared_ptr<ObImage> wind_Slash = RESOURCE->CreateSprite(L"Skill\\WinSlash_Large_Alt.png", Vec2(300.f / 5.f, 60.f) * 3.f, Int2(5, 1), true);
	wind_Slash->color.A(0.3f);
	wind_Slash->ChangeAnim(ANIMSTATE::ONCE, 0.05f);


	// Fire Particle
	{
		Vec2 fireSize = Vec2(768.f / 6.f, 512.f / 4.f) / 2.5f;
		shared_ptr<ObImage> fire1 = RESOURCE->CreateSprite(L"Particle\\Fire_Particle_6.bmp", fireSize, Int2(6, 4), true);
		fire1->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		fire1->frame.y = 0;

		shared_ptr<ObImage> fire2 = RESOURCE->CreateSprite(L"Particle\\Fire_Particle_6.bmp", fireSize, Int2(6, 4), true);
		fire2->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		fire2->frame.y = 1;

		shared_ptr<ObImage> fire3 = RESOURCE->CreateSprite(L"Particle\\Fire_Particle_6.bmp", fireSize, Int2(6, 4), true);
		fire3->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		fire3->frame.y = 2;

		shared_ptr<ObImage> fire4 = RESOURCE->CreateSprite(L"Particle\\Fire_Particle_6.bmp", fireSize, Int2(6, 4), true);
		fire4->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		fire4->frame.y = 3;

		m_Particles.insert({ L"Fire1", fire1 });
		m_Particles.insert({ L"Fire2", fire2 });
		m_Particles.insert({ L"Fire3", fire3 });
		m_Particles.insert({ L"Fire4", fire4 });
	}

	// StoneEffect
	{
		Vec2 stoneSize = Vec2(128.f / 4.f, 128.f / 4.f);
		shared_ptr<ObImage> stone1 = RESOURCE->CreateSprite(L"Particle\\stoneEffect_4x4.bmp", stoneSize, Int2(4, 4), true);
		stone1->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		stone1->frame.y = 0;

		shared_ptr<ObImage> stone2 = RESOURCE->CreateSprite(L"Particle\\stoneEffect_4x4.bmp", stoneSize, Int2(4, 4), true);
		stone2->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		stone2->frame.y = 1;

		shared_ptr<ObImage> stone3 = RESOURCE->CreateSprite(L"Particle\\stoneEffect_4x4.bmp", stoneSize, Int2(4, 4), true);
		stone3->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		stone3->frame.y = 2;

		shared_ptr<ObImage> stone4 = RESOURCE->CreateSprite(L"Particle\\stoneEffect_4x4.bmp", stoneSize, Int2(4, 4), true);
		stone4->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
		stone4->frame.y = 3;

		m_Particles.insert({ L"Stone1", stone1 });
		m_Particles.insert({ L"Stone2", stone2 });
		m_Particles.insert({ L"Stone3", stone3 });
		m_Particles.insert({ L"Stone4", stone4 });
	}

	{

		Vec2 telpoEffectSize = Vec2(766.f / 8.f, 512.f);
		shared_ptr<ObImage> telpoEffect = RESOURCE->CreateSprite(L"Particle\\teleportEffect2.bmp", Vec2(telpoEffectSize), Int2(8, 1), true);
		telpoEffect->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"StartEffect", telpoEffect });
	}

	// Effect
	m_Particles.insert({ L"HitEffect", hit_Effect });
	m_Particles.insert({ L"HitSpark", hit_Spark });
	m_Particles.insert({ L"HitSpark_Red", hit_Spark_Red });
	m_Particles.insert({ L"Fire_HitEffect", fire_hitEffect });
	m_Particles.insert({ L"Fire_HitEffect2", fire_hitEffect2 });

	// Player
	m_Particles.insert({ L"Wind_Slash", wind_Slash });
}

void CParticleManager::Boss_Resource_Settings()
{
	{
		Vec2 start_Lightning_Size = Vec2(350.f / 7.f, 100.f) * 2.f;
		shared_ptr<ObImage> start_Lightning = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_GirLand.png", start_Lightning_Size, Int2(7, 1), true);
		start_Lightning->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Start_Lightning", start_Lightning });
	}

	{
		Vec2 start_Lightning_Size = Vec2(280.f / 7.f, 88.f) * 2.f;
		shared_ptr<ObImage> start_Lightning = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_Land.png", start_Lightning_Size, Int2(7, 1), true);
		start_Lightning->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Particle_Land", start_Lightning });
	}

	{
		Vec2 size = Vec2(700.f / 7.f, 100.f) * 2.f;
		shared_ptr<ObImage> fist_Lightning = RESOURCE->CreateSprite(L"Monster\\Lightning\\Fist_All_F.png", size, Int2(7, 1), true);
		fist_Lightning->ChangeAnim(ANIMSTATE::ONCE, 0.05f);

		m_Particles.insert({ L"Fist_All", fist_Lightning });
	}

	{
		Vec2 size = Vec2(588.f / 6.f, 98.f) * 2.f;
		shared_ptr<ObImage> fist_Lightning_Y = RESOURCE->CreateSprite(L"Monster\\Lightning\\FistBasic_yellow.png", size, Int2(6, 1), true);
		fist_Lightning_Y->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Fist_Lightning_Y", fist_Lightning_Y });
	}

	{
		Vec2 size = Vec2(210.f / 6.f, 55.f) * 3.f;
		shared_ptr<ObImage> teleport_Floor = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_TeleportFloor.png", size, Int2(6, 1));
		teleport_Floor->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Teleport_Floor", teleport_Floor });
	}

	{
		Vec2 size = Vec2(150.f / 3.f, 50.f) * 3.f;
		shared_ptr<ObImage> shock_Spark = RESOURCE->CreateSprite(L"Monster\\Lightning\\ShockSpark.png", size, Int2(3, 1), true);
		shock_Spark->color = Color(0.5f, 0.7f, 0.3f, 0.5f);
		shock_Spark->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"ShockSpark", shock_Spark });
	}

	{
		Vec2 size = Vec2(179.f / 3.f, 53.f) * 3.f;
		shared_ptr<ObImage> shock_Spark = RESOURCE->CreateSprite(L"Monster\\Lightning\\Shock_Spark_Y.png", size, Int2(3, 1), true);
		shock_Spark->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"ShockSpark_Y", shock_Spark });
	}

	{
		Vec2 size = Vec2(141.f / 3.f, 33.f) * 3.f;
		shared_ptr<ObImage> shock_Spark = RESOURCE->CreateSprite(L"Monster\\Lightning\\Shock_AS.png", size, Int2(3, 1), true);
		shock_Spark->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"ShockSpark_AS", shock_Spark });
	}

	{
		Vec2 size = Vec2(299.f / 5.f, 132.f) * 2.f;
		shared_ptr<ObImage> img = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_Dash_Y.png", size, Int2(5, 1), true);
		img->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Light_Dash_Y", img });
	}

	{
		Vec2 size = Vec2(270.f / 6.f, 356.f) * 2.f;
		shared_ptr<ObImage> img = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_Long.png", size, Int2(6, 1), true);
		img->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Light_Long", img });
	}

	{
		Vec2 size = Vec2(280.f / 7.f, 88.f) * 2.f;
		shared_ptr<ObImage> img = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_Land.png", size, Int2(7, 1), true);
		img->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Light_Land", img });
	}

	{   
		Vec2 size = Vec2(112.f / 7.f, 64.f) * 2.f;
		shared_ptr<ObImage> img = RESOURCE->CreateSprite(L"Monster\\Lightning\\Lightning_Neddle_Loop.png", size, Int2(7, 1), true);
		img->ChangeAnim(ANIMSTATE::ONCE, 0.1f);

		m_Particles.insert({ L"Light_Neddle", img });
	}

}

shared_ptr<ObImage> CParticleManager::Spawn_Particle(const wstring& _Key, const Vec2& _Pos, Vec2 _Multiple)
{
	auto find_iter = m_Particles.find(_Key);
	if (find_iter == m_Particles.end()) return nullptr;

	auto particle = find_iter->second;
	auto newParticle = make_shared<ObImage>(*particle); // 복사 생성자

	newParticle->SetWorldPos(_Pos);

	if (_Multiple.x != -1)
		newParticle->scale.x = find_iter->second->scale.x * _Multiple.x;

	if (_Multiple.y != -1)
		newParticle->scale.y = find_iter->second->scale.y * _Multiple.y;

	m_ActiveParticles.push_back(newParticle);

	return newParticle;
}



