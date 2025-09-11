#pragma once

class CActor;

class CComponent
{
public:
	CComponent()		  = default;
	virtual ~CComponent() = default;

public:
	virtual void BeginPlay()		abstract {}
	virtual void TickComponent()	abstract {}
	virtual void Render()			abstract {}
	virtual void Release()			abstract {}

public:
	void	SetOwner(CActor* _Owner) { m_Owner = _Owner; }
	CActor* GetOwner()				 { return m_Owner; }

private:
	CActor* m_Owner = nullptr;
};

