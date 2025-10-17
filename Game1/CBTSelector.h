#pragma once

#include "CBTComposite.h"

class CBTSelector : public CBTComposite
{
public:
	CBTSelector() {}
	virtual ~CBTSelector() {}

public:
	BTResult Tick(BTContext& ctx) override;

private:

};

