#pragma once
class CCollisionManager
{
private:
	CCollisionManager() = default;
	~CCollisionManager() = default;

public:
	void		Init();
	void		Update();
public:
	static CCollisionManager* Get_Instance()
	{
		if (Instance == nullptr)
		{
			Instance = new CCollisionManager();
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
	static CCollisionManager* Instance;

public:
	void AddCollider(GameObject* collider);
	void RemoveCollider(GameObject* collider);

private:
	vector<GameObject*> m_Colliders;
};



