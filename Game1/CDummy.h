#pragma once

#include "CCharacter.h"

class CDummy : public CCharacter
{
public:
	CDummy();
	virtual ~CDummy();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	shared_ptr<ObRect> Get_DummyCol() { return m_DummyCol; }

protected:
	void Resource_Settings() override;

private:
	shared_ptr<ObRect> m_DummyCol;
	shared_ptr<ObImage> Dummy_Image;


};

