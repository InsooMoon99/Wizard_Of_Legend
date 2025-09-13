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
	void SetPos(Vector2 pos) { m_Pos = pos; }
	Vector2 GetPos() { return m_Pos; }

	void AddComponent(CComponent* _Component);
	void RemoveComponent(CComponent* _Component);

	void SetLayer(LayerType layer) { m_Layer = layer; }
	LayerType GetLayer() { return m_Layer; }

public:
	Vector2					m_Pos = { 0,0 };
	LayerType				m_Layer = LayerType::Layer_Object;
	vector<CComponent*>		m_Components;
};

