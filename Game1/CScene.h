#pragma once

class CScene
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
	CActor* m_Player;
	unordered_map<ESceneType, shared_ptr<ObImage>> m_Map;
};

