#pragma once

class CComponent;

class CActor
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
	void	Set_WorldPos(Vector2 pos) { m_Pos = pos; }
	Vector2	Get_WorldPos() { return m_Pos; }

	virtual void AddComponent(CComponent* _Component);
	virtual void RemoveComponent(CComponent* _Component);

	const vector<CComponent*>& GetAllComponents() const { return m_Components; }

	virtual shared_ptr<GameObject>  Get_Collision() { return m_Collision; }
	virtual shared_ptr<ObImage>		Get_CurImage() { return m_CurImg; }

protected:
	// Collision
	vector<GameObject*>		m_vecCol;

	shared_ptr<GameObject>	m_Collision = nullptr;
	shared_ptr<ObImage>		m_CurImg = nullptr;

	vector<CComponent*>		m_Components;
	Vector2					m_Pos = { 0,0 };

public:


};

