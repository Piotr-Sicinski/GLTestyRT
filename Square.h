//Square.h
#pragma once

#include "SceneObject.h"

#define DEFAULT_SQ_SIZE_BY_2 0.5

class Square : public SceneObject
{
public:
	Square(float a = 2 * DEFAULT_SQ_SIZE_BY_2);

	void reset();

	void transform(const Matrix4& rhs) override;
	Ray reflect(const Ray& incidantRay) const override;

	const Vector3& getNormal() const { return normal; }
	const Vector3* getCorners() const { return corners; }

	Vector3 intersect(const Line& line) const;		// intersect with a line/ray

protected:
	Vector3 normal;     // normal vector of a plane
	float d;            // coefficient of constant term: d = -(a*x0 + b*y0 + c*z0)
	float a;	//side length
	float aa;	//side length ^ 2
	Vector3 corners[4];
	Vector3 v1;
	Vector3 v2;

};

