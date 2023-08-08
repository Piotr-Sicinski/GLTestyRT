#include "Square.h"

// 3--2
// |  |
// |  |
// 0--1


Square::Square(float a) : a(a), aa(a* a)
{
	reset();
}

void Square::reset()
{
	corners[0].set(-a / 2, -a / 2, 0);
	corners[1].set(a / 2, -a / 2, 0);
	corners[2].set(a / 2, a / 2, 0);
	corners[3].set(-a / 2, a / 2, 0);

	transform(Matrix4());
}

void Square::transform(const Matrix4& rhs)
{
	corners[0] = rhs * corners[0];
	corners[1] = rhs * corners[1];
	corners[2] = rhs * corners[2];
	corners[3] = rhs * corners[3];

	v1 = corners[1] - corners[0];
	v2 = corners[3] - corners[0];
	normal = v2.cross(v1);
	d = -normal.dot(corners[0]);         // -(a*x0 + b*y0 + c*z0)

	//a = Vector3(corners[1] - corners[0]).length();
}

//using fact that ray is a chile of line
//	Vector3 intersect(const Ray& ray) const;

Vector3 Square::intersect(const Line& line) const
{
	// from line = p + t * v
	Vector3 p = line.getPoint();        // (x0, y0, z0)
	Vector3 v = line.getDirection();    // (x,  y,  z)

	// dot products
	float dot1 = normal.dot(p);         // a*x0 + b*y0 + c*z0
	float dot2 = normal.dot(v);         // a*x + b*y + c*z

	// if denominator=0, no intersect
	if (dot2 == 0)
		return NAN_VECTOR3;

	// find t = -(a*x0 + b*y0 + c*z0 + d) / (a*x + b*y + c*z)
	float t = -(dot1 + d) / dot2;

	if (t < 0)
		return NAN_VECTOR3;

	// find intersection point with plane
	p = p + (v * t);

	////checking, if intersection point is < a distant from each side od square
	//Line l;
	//for (int i = 0; i < 4; i++)
	//{
	//	l.set(corners[(i + 1) % 4] - corners[i], corners[i]);
	//	if (l.getDistance(p) > Vector3(corners[1] - corners[0]).length())
	//	{
	//		return NAN_VECTOR3;
	//	}
	//}
	//return p;

	dot1 = v1.dot(p - corners[0]);
	dot2 = v2.dot(p - corners[0]);

	if (0 > dot1 || dot1 > aa || 0 > dot2 || dot2 > aa)
	{
		return NAN_VECTOR3;
	}
	return p;
}

Ray Square::reflect(const Ray& incidantRay) const
{
	const Vector3 incidantRayDir = incidantRay.getDirection();
	Ray reflRay;

	reflRay.setPoint(intersect(incidantRay));

	if (isnan(reflRay.getPoint().x))
	{
		return reflRay;
	}


	Vector3 rotAxis = incidantRayDir.cross(normal);
	float halfRotAngle = incidantRayDir.angle(normal);
	Matrix4 m;
	m.rotate(2 * halfRotAngle, rotAxis);

	reflRay.setDirection(m * -incidantRayDir);

	return reflRay;
}
