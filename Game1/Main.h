#pragma once

class Main : public Scene
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override; 
	virtual void ResizeScreen() override;

private:
	shared_ptr<ObImage> m_Cursor;
};
