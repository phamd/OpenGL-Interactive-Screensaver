#include "calculations.h"

float randFloat(float s, float t) {
	return s + (t - s) * ((float)rand()) / (float)RAND_MAX;
}

Vector2d pointSlope(float sx, float sy, float fx, float fy)
{
	float deltaX = fx - sx;
	float deltaY = fy - sy;
	float length = sqrt(deltaX*deltaX + deltaY*deltaY);
	printf("deltaX: %f; deltaY: %f; length %f; xdiff = %f; ydiff = %f\n ",
		deltaX, deltaY, length, deltaX / length, deltaY / length); //debug
	return Vector2d(deltaX / length, deltaY / length);
}

Vector3d randomColor(void)
{
	return Vector3d((float)rand() / float(RAND_MAX), (float)rand() / float(RAND_MAX), (float)rand() / float(RAND_MAX));
}