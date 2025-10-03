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

private:
	vector<shared_ptr<ObImage>> m_Images;
	vector<shared_ptr<ObRect>> m_TextRect;

	shared_ptr<ObImage> m_Title_Image;
	shared_ptr<ObImage> m_Title_Text;
	shared_ptr<ObImage> m_Title_Shadow;

	shared_ptr<ObImage> m_LoadOutSelectStart;
	shared_ptr<ObImage> m_LoadOutSelectEdit;
	shared_ptr<ObImage> m_LoadOutSelectExit;

	vector<EStartStageType> m_Settings;

	EStartStageType StartType = EStartStageType::Pressed;

	RECT m_DrawRect[4];
	Color m_ChangeColor[4];
	wstring m_Font = L"Noto Sans";

	bool bStartCheck = false;
	bool bVisible = false;

	float m_UpTimer = 0.f;

	int m_LastHovered = -1;
};

