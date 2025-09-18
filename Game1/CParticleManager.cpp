#include "stdafx.h"
#include "CParticleManager.h"
#include "CResourceManager.h"

CParticleManager* CParticleManager::Instance = nullptr;

void CParticleManager::Init()
{
	Resource_Settings();
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

	m_Particles.insert({ L"HitEffect", hit_Effect });
	m_Particles.insert({ L"HitSpark", hit_Spark });
}

shared_ptr<ObImage> CParticleManager::Spawn_Particle(const wstring& _Key, const Vec2& _Pos)
{
	auto find_iter = m_Particles.find(_Key);
	if (find_iter == m_Particles.end()) return nullptr;
	
	auto particle = find_iter->second;
	auto newParticle = make_shared<ObImage>(*particle); // 복사 생성자
	
	newParticle->SetWorldPos(_Pos);
	
	m_ActiveParticles.push_back(newParticle);
	
	return newParticle;
}



