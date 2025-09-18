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

protected:
	void Resource_Settings() override;

private:
	void SetSprite(EPlayerState _State, EPlayerDir _Dir) override;

private:
	shared_ptr<ObImage> Dummy_Image;


};

