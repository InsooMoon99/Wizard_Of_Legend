#pragma once

struct AnimDesc
{
	int		line;
	float	interval;
};

struct RenderEntry
{
	int		 Z_Order;
	ObImage* Img;
};