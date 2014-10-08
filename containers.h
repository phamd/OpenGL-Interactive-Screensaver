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
	Vector3d colour;
	Vertex(Vector2d, Vector2d);
	Vertex();
};

class Shapes
{
public:
	std::vector<Vertex> vertices; // Shapes are made of vertices
	unsigned int type; // Three types: Dot, Line, Poly
	Shapes(Vertex);
	Shapes(std::vector<Vertex>);
	Shapes();
};

#endif
