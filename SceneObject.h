//SceneObject.h
#pragma once
#include "Plane.h"
#include "Matrices.h"
#include "Ray.h"

class SceneObject {
public:
	virtual void transform(const Matrix4& rhs) {};
	virtual Ray reflect(const Ray& incidantRay) const
	{
		return Ray();
	};
};

