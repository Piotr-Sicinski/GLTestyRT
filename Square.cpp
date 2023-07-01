#include "Square.h"


Square::Square(float a) : a(a)
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
		return Vector3(NAN, NAN, NAN);

	// find t = -(a*x0 + b*y0 + c*z0 + d) / (a*x + b*y + c*z)
	float t = -(dot1 + d) / dot2;

	// find intersection point with plane
	p = p + (v * t);

	//// move p in local system
	//p -= corners[0];

	////checking set of linear equasions to know, if intersected point is inside square
	//float n1 = -p.y * v2.x + p.x * v2.y;    //numerator1
	//float n2 = p.y * v1.x - p.x * v1.y;    //numerator2
	//float d = -v1.y * v2.x + v1.x * v2.y;    //denominator

	////float n1 = -p.z * v2.x + p.x * v2.z;    //numerator1
	////float n2 = p.z * v1.x - p.x * v1.z;    //numerator2
	////float d = -v1.z * v2.x + v1.x * v2.z;    //denominator

	////float n1 = -p.z * v2.y + p.y * v2.z;    //numerator1
	////float n2 = p.z * v1.y - p.y * v1.z;    //numerator2
	////float d = -v1.z * v2.y + v1.y * v2.z;    //denominator

	//float a = n1 / d;
	//float b = n2 / d;

	////0 <= n/d <= 1
	//if (0 <= a && a <= 1 && 0 <= b && b <= 1)
	//{
	//	p += corners[0];
	//	// find intersection point within sides
	//	return p;
	//}

	//return NAN_VECTOR3;

	Line l;
	for (int i = 0; i < 4; i++)
	{
		l.set(corners[(i + 1) % 4] - corners[i], corners[i]);
		if (l.getDistance(p) > Vector3(corners[1] - corners[0]).length())
		{
			return NAN_VECTOR3;
		}
	}
	return p;

}

Vector3 Square::intersect(const Ray& ray) const
{
	// from line = p + t * v
	Vector3 p = ray.getPoint();        // (x0, y0, z0)
	Vector3 v = ray.getDirection();    // (x,  y,  z)

	// dot products
	float dot1 = normal.dot(p);         // a*x0 + b*y0 + c*z0
	float dot2 = normal.dot(v);         // a*x + b*y + c*z

	// if denominator=0, no intersect
	if (dot2 == 0)
		return Vector3(NAN, NAN, NAN);

	// find t = -(a*x0 + b*y0 + c*z0 + d) / (a*x + b*y + c*z)
	float t = -(dot1 + d) / dot2;

	if (t < 0)
		return Vector3(NAN, NAN, NAN);

	// find intersection point with plane
	p = p + (v * t);

	Line l;
	for (int i = 0; i < 4; i++)
	{
		l.set(corners[(i + 1) % 4] - corners[i], corners[i]);
		if (l.getDistance(p) > Vector3(corners[1] - corners[0]).length())
		{
			return NAN_VECTOR3;
		}
	}
	return p;
}

Ray Square::reflect(const Ray& incidantRay) const
{
	const Vector3 incidantRayDir = incidantRay.getDirection();
	Ray reflRay;

	reflRay.setPoint(intersect(incidantRay));

	if (reflRay.getPoint() == NAN_VECTOR3)
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
