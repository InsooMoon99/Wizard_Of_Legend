#pragma once

#include "CScene.h"

class CStage_Start : public CScene
{
public:
	CStage_Start();
	virtual ~CStage_Start() override;

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;
};

