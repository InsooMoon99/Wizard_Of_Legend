#pragma once

#include "CScene.h"

class CStage02 : public CScene
{
public:
	CStage02();
	virtual ~CStage02() override;

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;
};

