#pragma once

#include "Matrices.h"
#include "Ray.h"

class SceneObject {
public:
	virtual void transform(const Matrix4& rhs) {};
	virtual Ray reflect(const Ray& incidantRay) const
	{
		return Ray();
	};
	//virtual Vector3 intersect(const Ray& ray) const = 0;

	virtual Vector3 intersect(const Ray& ray) const
	{
		return Vector3();
	};

	void setReflRough(uint16_t refl, uint16_t rough) {
		if (refl >= 0 && rough >= 0 && refl <= REFL_MAX && rough <= ROUGH_MAX) {
			reflectiveness = refl;
			roughness = rough;
		}
		else {
			raiseErr(InvalidValues);
		}
	}


	static constexpr uint16_t DEFAULT_REFLECTIVENESS = REFL_MAX / 2;
	static constexpr uint16_t DEFAULT_ROUGHNESS = ROUGH_MAX / 2;

	uint16_t reflectiveness = DEFAULT_REFLECTIVENESS;
	uint16_t roughness = DEFAULT_ROUGHNESS;
};

