#pragma once
#include "Line.h"
#include "RT.h"

class Ray : public Line
{
public:
	Ray() : Line() {}
	Ray(const Vector3& v, const Vector3& p, uint32_t pow) : Line(v, p), power(pow) {}

	uint32_t power = POWER_MAX;
protected:

};

