#pragma once
#include "Frame.h"
#include "SceneObject.h"

class Render
{
public:
	Render();

	void swapFrames();
	void renderFrame();
	uint8_t castRay(const Ray& r);

	Ray rays[MAX_RAY_COUNT];
	int rayN = 0;

	Frame ready, working;
	Line camera;
	float screenDist;
};

