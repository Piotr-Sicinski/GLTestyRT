#pragma once
#include "Line.h"
#include "Common.h"

class Ray : public Line
{
public:
	Ray() : Line(), power(POWER_MAX) {}
	Ray(const Vector3& v, const Vector3& p, uint32_t pow = POWER_MAX) : Line(v, p), power(pow) {}

	uint32_t power;
protected:

};

