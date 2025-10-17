#include "stdafx.h"
#include "CBTComposite.h"

void CBTComposite::TickServices(BTContext& ctx, float delta)
{
	for (auto& service : m_Services)
	{
		if (service)
			service->Tick(ctx, delta);

	}
}
