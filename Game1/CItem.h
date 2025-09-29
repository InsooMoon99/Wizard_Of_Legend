#pragma once

#include "CObject.h"
#include "CActor.h"

class CItem : public CActor
{
public:
	CItem();
	~CItem() override;

public:
	void BeginPlay() override;
	void Tick() override;
	void Render() override;
	void LateUpdate() override;
	void Release() override;

public:
	void SetData(const FItemData& data);
	void ApplyData();

	const FItemData& GetData() const { return m_ItemData; }
	const string& GetIconKey() const { return m_ItemData.IconKey; }
	Vec2 GetIconSize() const { return Vec2(m_ItemData.IconW, m_ItemData.IconH); }

public:
	virtual void ActivateItem(CActor* actor) abstract {};
	virtual bool IsDestroy() { return m_PendingKill; }
	void DisableAndDestroy();


public:
	virtual void OnComponentBeginOverlap(GameObject* collider, GameObject* other) override {};
	virtual void OnComponentEndOverlap(GameObject* collider, GameObject* other) override {};



protected:
	FItemData m_ItemData;

	shared_ptr<ObImage> m_ItemImg;
	Vec2 m_Size {};

	bool m_PendingKill = false;
};


