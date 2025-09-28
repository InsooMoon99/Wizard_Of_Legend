#pragma once

class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void BeginPlay()  abstract {};
	virtual void Tick()		  abstract {};
	virtual void LateUpdate() abstract {};
	virtual void Render()	  abstract {};
	virtual void Release()	  abstract {};

};

