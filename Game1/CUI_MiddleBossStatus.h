#pragma once

#include "CUI.h"

class CUI_MiddleBossStatus : public CUI
{
public:
	CUI_MiddleBossStatus();
	virtual ~CUI_MiddleBossStatus();

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

	void SetStatus(CStatusComponent* _Status) { m_Status = _Status; }

private:
	vector<shared_ptr<ObImage>> m_BossStatus;

	shared_ptr<ObImage> m_Background;
	shared_ptr<ObImage> m_Hp;

	CStatusComponent* m_Status;

	Vec2 m_HpScale = Vec2(178.f * 1.41f, 14.f * 1.6f);

	shared_ptr<ObImage> m_Hp_back;
	float m_InstantRatio = 1.0f;  
	float m_HurtRatio = 1.0f;

	float m_HurtLerpSpeed = 1.0f;
};

