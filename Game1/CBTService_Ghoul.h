#pragma once
#include "CBTService.h"
class CBTService_Ghoul : public CBTService
{
public:
	CBTService_Ghoul(float periodSec = 0.2f);
	virtual ~CBTService_Ghoul();

public:
	void OnTick(BTContext& ctx) override;

private:


};

