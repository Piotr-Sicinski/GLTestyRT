#include "Cube.h"

Cube::Cube(float a) : a(a)
{
	FOR6i
	{
		sides[i] = Square(a);
	}
	reset();

}

void Cube::reset()
{
	FOR6i
	{
		sides[i].reset();
	}

	Matrix4 m;
	m.translate(0, 0, a / 2);
	FOR6i
	{
		sides[i] = Square(a);
		sides[i].transform(m);
	}
	sides[1].transform(Matrix4().rotateY(90));
	sides[2].transform(Matrix4().rotateY(180));
	sides[3].transform(Matrix4().rotateY(-90));
	sides[4].transform(Matrix4().rotateX(90));
	sides[5].transform(Matrix4().rotateX(-90));
}

void Cube::transform(const Matrix4& rhs)
{
	FOR6i
	{
		sides[i].transform(rhs);
	}
}

Ray Cube::reflect(const Ray& incidantRay) const
{
	const Vector3 incidantRayPoint = incidantRay.getPoint();
	Ray reflRay;
	Ray bestRay(NAN_VECTOR3, NAN_VECTOR3);
	bool cond;

	FOR6i
	{
		reflRay = sides[i].reflect(incidantRay);

		if (std::isnan(reflRay.getPoint().x))
		{
			continue;
		}

		//true if no bestRay or newly calculated ray is reflected erlier than best
		if (std::isnan(bestRay.getPoint().x) || incidantRayPoint.distance(bestRay.getPoint()) > incidantRayPoint.distance(reflRay.getPoint()))
		{
			bestRay = reflRay;
			continue;
		}

	}
	return bestRay;
}
