#pragma once

class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void BeginPlay() abstract;
	virtual void Tick()		 abstract;
	virtual void Render()	 abstract;

};

