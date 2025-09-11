#pragma once

#include "CScene.h"

class CPlayer;
class CActor;

class CStage01 : public CScene
{
public:
	CStage01();
	virtual ~CStage01() override;

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

private:
	CActor* player;
};

