#pragma once

#include "CGameObject.h"

class CComponent;

class CActor : public CGameObject
{
public:
	CActor();
	virtual ~CActor();

public:
	virtual void BeginPlay();
	virtual void Tick();
	virtual void LateUpdate() {};
	virtual void Render();
	virtual void Release();

public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other);
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other);

public:
	void	Set_WorldPos(Vector2 pos)
	{
		m_Pos = pos;
		if (m_Collision)
			m_Collision->SetWorldPos(pos);
	}

	// 위치 세팅이 된 후 소환되게
	void SetSpawnPos(const Vec2& pos)
	{
		m_PendingSpawnPos = pos;
		m_HasPendingSpawnPos = true;
	}

	Vector2	Get_WorldPos() { return m_Pos; }

	void ApplyPendingSpawnPos();

public:
	virtual void AddComponent(shared_ptr<CComponent> _Component);
	virtual void RemoveComponent(CComponent* _Component);

	const vector<shared_ptr<CComponent>>& GetAllComponents() const { return m_Components; }

	virtual shared_ptr<GameObject>  Get_Collision() { return m_Collision; }
	virtual shared_ptr<ObImage>		Get_CurImg() { return m_CurImg; }

	void SetPendingKill(bool _Pending) { m_PendingKill = _Pending; }
	bool GetPendingKill() { return m_PendingKill; }

public:
	bool	IsDestroy() const { return m_PendingKill; }

protected:
	bool	m_PendingKill = false;

	vector<shared_ptr<CComponent>>		m_Components;
	// Collision
	vector<GameObject*>		m_vecCol;
	shared_ptr<GameObject>	m_Collision = nullptr;
	shared_ptr<ObImage>		m_CurImg;

	Vector2 m_Pos = { 0,0 };
	Vec2	m_PendingSpawnPos;
	bool    m_HasPendingSpawnPos = false;
};

