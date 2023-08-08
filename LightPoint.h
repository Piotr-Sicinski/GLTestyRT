#pragma once
#include "Common.h"
#include "Vectors.h"

class LightPoint
{
public:
	LightPoint(uint32_t pow = POWER_MAX) : power(pow) {}
	LightPoint(const Vector3& p, uint32_t pow = POWER_MAX) : point(p), power(pow) {}

	uint32_t power;
	Vector3 point;
};

