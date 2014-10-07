#ifndef __CALCULATIONS_H__
#define __CALCULATIONS_H__

#include "containers.h"

float randFloat(float s, float t);

Vector2d pointSlope(Vector2d s, Vector2d f);
Vector2d pointSlope(float sx, float sy, float fx, float fy);

Vector3d randomColour(void);
Vector3d getColour(Vector3d colour);

#endif
