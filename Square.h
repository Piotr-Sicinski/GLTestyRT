#ifndef SQUARE_H
#define SQUARE_H

#include "Plane.h"
#include "Matrices.h"


class Square
{
public:
	Square();

	void move(const Matrix4& rhs);

	const Vector3& getNormal() const { return normal; }

	Vector3 intersect(const Line& line) const;              // intersect with a line


protected:
	static constexpr uint16_t DEFAULT_REFLECTIVENESS = REFL_MAX / 2;
	static constexpr uint16_t DEFAULT_ROUGHNESS = ROUGH_MAX / 2;

	uint16_t reflectiveness;
	uint16_t roughness;


	Vector3 normal;     // normal vector of a plane
	float d;            // coefficient of constant term: d = -(a*x0 + b*y0 + c*z0)
	float a;	//side length
	Vector3 corner[3];
	Vector3 v1;
	Vector3 v2;

};




#endif