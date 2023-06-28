#include "Square.h"

Square::Square()
{
}

void Square::move(const Matrix4& rhs)
{
	corner[0] = rhs * corner[0];
	corner[1] = rhs * corner[1];
	corner[2] = rhs * corner[2];

	v1 = corner[1] - corner[0];
	v2 = corner[2] - corner[0];
	normal = v2.cross(v1);
	d = -normal.dot(corner[0]);         // -(a*x0 + b*y0 + c*z0)
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

	//checking set of linear equasions to know, if intersected point is inside square
	float n1 = -p.y * v2.x + p.x * v2.y;    //numerator1
	float n2 = p.y * v1.x - p.x * v1.y;    //numerator2
	float d = v1.y * v2.x - v1.x * v2.y;    //denominator

	//0 <= n/d <= 1

	if (0 <= n1 <= d && 0 <= n2 <= d)
	{
		// find intersection point within sides
		return p + (v * t);
	}
	else
	{
		return Vector3(NAN, NAN, NAN);
	}

}
