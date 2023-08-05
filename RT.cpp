#include "RT.h"


void fillPattern(uint8_t image[RT_RENDER_RES][RT_RENDER_RES])
{
	int value = 0;
	for (int row = 0; row < RT_RENDER_RES; row++)
	{
		value = row % COLOR_DEPTH;

		for (int col = 0; col < RT_RENDER_RES; col++)
		{
			image[row][col] = value % COLOR_DEPTH;
			value += 1; // Increment the value
		}
	}
}

void fillTexture(uint8_t image[RT_RENDER_RES][RT_RENDER_RES], uint8_t texture[RT_RENDER_RES * RT_RENDER_RES * 3])
{
	for (int row = 0; row < RT_RENDER_RES; row++)
	{
		for (int col = 0; col < RT_RENDER_RES; col++)
		{
			texture[3 * (row * RT_RENDER_RES + col)] = image[row][col];
			texture[3 * (row * RT_RENDER_RES + col) + 1] = image[row][col];
			texture[3 * (row * RT_RENDER_RES + col) + 2] = image[row][col];
		}
	}
}


