#include "RT.h"

void fillPattern(int image[RT_SCREEN_RES][RT_SCREEN_RES])
{
	int value = 0;
	for (int row = 0; row < RT_SCREEN_RES; row++)
	{
		value = row % RT_SCREEN_RES;

		for (int col = 0; col < RT_SCREEN_RES; col++)
		{
			image[row][col] = value % RT_SCREEN_RES;
			value += 1; // Increment the value
		}
	}
}

