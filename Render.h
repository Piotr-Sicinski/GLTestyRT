#pragma once
#include "Frame.h"
#include "SceneObject.h"
#include "RT.h"

class Render
{
public:
	Render();

	void swapFrames();
	void renderFrame();
	void castRay(Ray r);

	Ray rays[MAX_RAY_COUNT];
	int rayN = 0;



	Frame ready, working;
	Line camera;
	float screenDist;







};

