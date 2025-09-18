#pragma once

#include "CACtor.h"
#include "CEnums.h"

class CTile : public CActor
{
public:
	CTile();
	virtual ~CTile();

public:
	void BeginPlay() override;
	void Tick() override;
	void LateUpdate() override;
	void Render() override;
	void Release() override;

public:
	void Set_DrawID(int _DrawID) { m_DrawID = _DrawID; }
	void Set_Option(int _Option) { m_Option = _Option; }

	int& Get_DrawID() { return m_DrawID; }
	int& Get_OptionID() { return m_Option; }

	ETileType GetTileType() { return m_TileType; }
public:

private:
	unordered_map<ETileType, shared_ptr<ObImage>> m_Tiles;
	ETileType	m_TileType;

	Vec2	m_Size;
	int     m_DrawID = 0;
	int     m_Option = 0;


};

