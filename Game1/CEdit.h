#pragma once

#include "CScene.h"

class CEdit : public CScene
{
public:
	CEdit();
	virtual ~CEdit();

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;
};

