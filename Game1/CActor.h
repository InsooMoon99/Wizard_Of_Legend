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
	void Set_WorldPos(Vector2 pos) { m_Pos = pos; }
	Vector2 Get_WorldPos() { return m_Pos; }

	virtual void AddComponent(CComponent* _Component);
	virtual void RemoveComponent(CComponent* _Component);

	const vector<CComponent*>& GetAllComponents() const { return m_Components; }

	void SetLayer(ELayerType layer) { m_Layer = layer; }
	ELayerType GetLayer() { return m_Layer; }

public:
	Vector2					m_Pos = { 0,0 };
	ELayerType				m_Layer = ELayerType::Layer_Object;
	vector<CComponent*>		m_Components;
};

