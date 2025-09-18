#pragma once

class IColorChanger
{
public:
	IColorChanger() = default;
	virtual ~IColorChanger() = default;

public:
	virtual void Update_Hit()	abstract {};

	virtual void Hit_Color()	abstract {};
	virtual void Hit_Effect()	abstract {};
};

