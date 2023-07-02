//Square.h
#pragma once

#include "SceneObject.h";

#define DEFAULT_SQ_SIZE_BY_2 0.5

class Square
{
public:
	Square(float a = 2 * DEFAULT_SQ_SIZE_BY_2);

	void reset();

	void transform(const Matrix4& rhs);

	const Vector3& getNormal() const { return normal; }
	const Vector3* getCorners() const { return corners; }


	Vector3 intersect(const Line& line) const;              // intersect with a line
	Vector3 intersect(const Ray& ray) const;              // intersect with a ray

	Ray reflect(const Ray& incidantRay) const;

protected:
	static constexpr uint16_t DEFAULT_REFLECTIVENESS = REFL_MAX / 2;
	static constexpr uint16_t DEFAULT_ROUGHNESS = ROUGH_MAX / 2;

	uint16_t reflectiveness;
	uint16_t roughness;


	Vector3 normal;     // normal vector of a plane
	float d;            // coefficient of constant term: d = -(a*x0 + b*y0 + c*z0)
	float a;	//side length
	Vector3 corners[4];
	Vector3 v1;
	Vector3 v2;

};

