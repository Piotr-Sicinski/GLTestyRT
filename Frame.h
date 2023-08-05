#pragma once
#include "Common.h"

class Frame
{
public:
	Frame() {}
	uint8_t image[RT_RENDER_RES][RT_RENDER_RES] = { 0 };
};

