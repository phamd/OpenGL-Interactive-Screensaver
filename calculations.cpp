#include "calculations.h"
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <time.h>

float randFloat(float s, float t) {
	return s + (t - s) * ((float)rand()) / (float)RAND_MAX;
}

Vector2d pointSlope(Vector2d s, Vector2d f)
{
	return pointSlope(s.x, s.y, f.x, f.y);
}

Vector2d pointSlope(float sx, float sy, float fx, float fy)
{
	float deltaX = fx - sx;
	float deltaY = fy - sy;
	float length = sqrt(deltaX*deltaX + deltaY*deltaY);
	float returnX = (deltaX == 0) ? 0 : deltaX / length;
	float returnY = (deltaY == 0) ? 0 : deltaY / length;
	//printf("deltaX: %f; deltaY: %f; length %f; xdiff = %f; ydiff = %f\n ", deltaX, deltaY, length, returnX, returnY); //debug
	return Vector2d(returnX, returnY);
}

Vector3d randomColour(void)
{
	return Vector3d((float)rand() / float(RAND_MAX), (float)rand() / float(RAND_MAX), (float)rand() / float(RAND_MAX));
}

Vector3d getColour(Vector3d colour)
{
	if (colour.x == -1)
		return randomColour();
	else
		return colour;
}

