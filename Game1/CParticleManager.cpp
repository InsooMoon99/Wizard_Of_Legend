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

	m_Particles.insert({ L"Particle", hit_Effect });
}

shared_ptr<ObImage> CParticleManager::Spawn_Particle(const wstring& _Key, const Vec2& _Pos)
{
	auto find_iter = m_Particles.find(_Key);
	if (find_iter == m_Particles.end()) return nullptr;
	
	auto particle = find_iter->second;
	auto newParticle = make_shared<ObImage>(*particle);
	
	newParticle->SetWorldPos(_Pos);
	
	m_ActiveParticles.push_back(newParticle);
	
	return newParticle;
}



