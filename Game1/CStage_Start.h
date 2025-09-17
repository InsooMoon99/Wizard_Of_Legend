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

	shared_ptr<ObImage> m_Title_Image;
	shared_ptr<ObImage> m_Title_Text;

	shared_ptr<ObImage> m_Test_Start;
	shared_ptr<ObImage> m_Test_Edit;
	shared_ptr<ObImage> m_Test_Exit;
};

