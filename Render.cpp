#include "Render.h"
#include "LightPoint.h"

extern float cameraAngleX, cameraAngleY, cameraX, cameraY, cameraDistance;

extern SceneObject* objects[MAX_OBJ_COUNT];
extern int objN;
extern LightPoint* lights[MAX_LGH_COUNT];
extern int lghN;

extern int deg;
extern uint8_t image[RT_RENDER_RES][RT_RENDER_RES];


Render::Render()
{
	screenDist = RT_RENDER_RES / tan(FOV * DEG2RAD / 2.0f);
}

void Render::swapFrames() {
	// Swap the 'ready' and 'working' frames using a temporary variable
	static Frame temp;
	temp = ready;
	ready = working;
	working = temp;
}

void Render::renderFrame()
{
	static Ray beginRay;
	static Matrix4 camRotM;
	static Vector3 beginRayDir, beginPoint,
		pixelDeltaX, pixelDeltaY;

	camRotM.identity();
	//camRotM.translate(0, 0, cameraDistance);
	camRotM.rotateX(-cameraAngleX);
	camRotM.rotateY(-cameraAngleY);

	beginPoint = camRotM * Vector3(0, 0, cameraDistance);
	//mo¿na uproœciæ daj¹c -beginPoint
	camera.set(camRotM * Vector3(0, 0, -1), beginPoint);

	//step of 2, because of 1 ofset in the begining corner
	pixelDeltaX = camRotM * Vector3(2, 0, 0);
	pixelDeltaY = camRotM * Vector3(0, 2, 0);

	beginRay.setPoint(beginPoint);
	beginRay.setDirection(camRotM * Vector3(-RT_RENDER_RES + 1, -RT_RENDER_RES + 1, -screenDist));

	for (int row = 0; row < RT_RENDER_RES; row++)
	{
		for (int col = 0; col < RT_RENDER_RES; col++)
		{
			//working.image[row][col] = image[row][col] + deg;


			working.image[row][col] = castRay(beginRay);

			beginRay.direction += pixelDeltaY;
		}
		beginRay.direction -= RT_RENDER_RES * pixelDeltaY;
		beginRay.direction += pixelDeltaX;
	}

	swapFrames();
}

uint8_t Render::castRay(const Ray& r)
{
	static Ray reflRay;
	for (int i = 0; i < objN; i++)
	{
		reflRay = (*objects[i]).reflect(r);

		if (!isnan(reflRay.getPoint().x))
		{
			return 255;
		}
	}

	return 0;
}
