#pragma once

#include "CScene.h"

class CPlayer;
class CActor;
class CDummy;

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

public:
	void Resource_Setting();

private:
	vector<CActor*> m_Actors;

	Vec2 m_PlayerPos;
	Vec2 m_MapSize;

	unordered_map<ESceneType, shared_ptr<ObImage>> m_Map;
	vector<shared_ptr<ObImage>> m_Objects;

	CActor* m_TestDummy;


};

