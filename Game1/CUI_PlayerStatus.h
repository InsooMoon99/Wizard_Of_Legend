#pragma once

#include "CUI.h"

class CUI_PlayerStatus : public CUI
{
public:
	CUI_PlayerStatus();
	virtual ~CUI_PlayerStatus();

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void Release() override;

	void SetStatus(CStatusComponent* _Status) { m_Status = _Status; }

private:
	vector<shared_ptr<ObImage>> m_PlayerStatus;

	shared_ptr<ObImage> m_Background;
	shared_ptr<ObImage> m_Hp;
	shared_ptr<ObImage> m_Mp;
	shared_ptr<ObImage> m_Gold;

	CStatusComponent* m_Status;

	float m_HpScaleX = 122.f * 2.f;
	float m_MpScaleX = 96.f * 2.f;
};

