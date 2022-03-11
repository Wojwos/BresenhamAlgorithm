#include <math.h>
#include "BresenhamC.h"

void getPixelsPositionC(bool **pixels, int x1, int y1, int x2, int y2) {
	int dx = 0, dy = 0;
	int xi = 0, yi = 0;
	int ai = 0, bi = 0, d = 0;
	int x = x1, y = y1;

	if (x1 < x2)
	{
		xi = 1;
		dx = x2 - x1;
	}
	else
	{
		xi = -1;
		dx = x1 - x2;
	}

	if (y1 < y2)
	{
		yi = 1;
		dy = y2 - y1;
	}
	else
	{
		yi = -1;
		dy = y1 - y2;
	}

	pixels[x][y] = 1;

	if (dx > dy)
	{
		ai = (dy - dx) * 2;
		bi = dy * 2;
		d = bi - dx;
		while (x != x2)
		{
			if (d >= 0)
			{
				x += xi;
				y += yi;
				d += ai;
			}
			else
			{
				d += bi;
				x += xi;
			}
			pixels[x][y] = 1;
		}
	}
	else
	{
		ai = (dx - dy) * 2;
		bi = dx * 2;
		d = bi - dy;
		while (y != y2)
		{
			if (d >= 0)
			{
				x += xi;
				y += yi;
				d += ai;
			}
			else
			{
				d += bi;
				y += yi;
			}
			pixels[x][y] = 1;
		}
	}
}