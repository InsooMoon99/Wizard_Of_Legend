#pragma once

#include "CBTService.h"

class CAIComponent;

class CBTService_Ghoul : public CBTService
{
public:
	CBTService_Ghoul(float periodSec = 0.2f);
	virtual ~CBTService_Ghoul();

public:
	void OnTick(BTContext& ctx) override;

private:
	shared_ptr<CAIComponent> m_AIComponent;

};

