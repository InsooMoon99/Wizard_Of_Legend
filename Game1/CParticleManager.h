#pragma once

class CParticleManager
{
private:
	CParticleManager() = default;
	~CParticleManager() = default;

public:
	void		Init();
	void		Update();
	void		Render();
	void		Release();

	static CParticleManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CParticleManager();
		}

		return Instance;
	}

	static void Destroy_Instance()
	{
		if (Instance)
		{
			delete Instance;
			Instance = nullptr;
		}
	}

private:
	static CParticleManager* Instance;

public:
	void Resource_Settings();
	void Boss_Resource_Settings();

	shared_ptr<ObImage> Spawn_Particle(const wstring& _Key, const Vec2& _Pos, Vec2 _Multiple = Vec2(-1, -1));

private:
	unordered_map<wstring, shared_ptr<ObImage>> m_Particles;
	vector<shared_ptr<ObImage>> m_ActiveParticles;
};

