#pragma once
#include "Square.h"

#define FOR6i for (int i = 0; i < 6; i++)

class Cube
{
public:
	Cube(float a = 2 * DEFAULT_SQ_SIZE_BY_2);

	void reset();
	void transform(const Matrix4& rhs);

	Ray reflect(const Ray& incidantRay) const;

	//protected:
	Square sides[6];
	float a;
};

