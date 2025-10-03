#pragma once

#include "CGameObject.h"

class CScene : CGameObject
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Init() abstract       {}
	virtual void Update() abstract     {}
	virtual void LateUpdate() abstract {}
	virtual void Render() abstract     {}
	virtual void Release()abstract     {}

protected:
	shared_ptr<CActor> m_Player;
	unordered_map<ESceneType, shared_ptr<ObImage>> m_Map;
};

