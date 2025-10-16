#include "stdafx.h"
#include "CBTComposite.h"

void CBTComposite::TickServices(BTContext& ctx, float delta)
{
	for (auto& service : m_Services)
		service->Tick(ctx, delta);
}
