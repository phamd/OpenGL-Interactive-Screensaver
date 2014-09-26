#ifndef __CONTAINERS_H__
#define __CONTAINERS_H__

#include <vector>

class Vector2d
{
public:
	float x;
	float y;
	Vector2d(float, float);
	Vector2d();
};

class Vector3d
{
public:
	float x;
	float y;
	float z;
	Vector3d(float, float, float);
	Vector3d();
};

class Vertex
{
public:
	Vector2d position;
	Vector2d direction;
	Vector3d color;
	Vertex(Vector2d, Vector2d);
	Vertex();
};

class Drawable 
{
public:
	std::vector<Vertex> vertices;
	int type; 
	Drawable(Vertex);
	Drawable(std::vector<Vertex>);
	Drawable();
};

#endif
