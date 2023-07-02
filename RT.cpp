#include "RT.h"

void fillPattern(int image[RT_SCREEN_RES][RT_SCREEN_RES])
{
	int value = 0;
	for (int row = 0; row < RT_SCREEN_RES; row++)
	{
		value = row % COLOR_DEPTH;

		for (int col = 0; col < RT_SCREEN_RES; col++)
		{
			image[row][col] = value % COLOR_DEPTH;
			value += 1; // Increment the value
		}
	}
}

