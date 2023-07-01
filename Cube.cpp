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
	return Ray();
}
