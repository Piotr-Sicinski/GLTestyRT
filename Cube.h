//Cube.h
#pragma once
#include "SceneObject.h"
#include "Square.h"

#define FOR6i for (int i = 0; i < 6; i++)

class Cube : public SceneObject
{
public:
	Cube(float a = 2 * DEFAULT_SQ_SIZE_BY_2);

	void reset();
	void transform(const Matrix4& rhs) override;

	Ray reflect(const Ray& incidantRay) const override;

	//protected:
	Square sides[6];
	float a;
};

