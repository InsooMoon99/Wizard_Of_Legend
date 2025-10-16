#pragma once
#include "CBTComposite.h"
class CBTSequence : public CBTComposite
{
public:
	CBTSequence() {}
	virtual ~CBTSequence() {}

public:
	BTResult Tick(BTContext& ctx) override;

private:

};

