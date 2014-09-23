#ifndef __CONTAINERS_H__
#define __CONTAINERS_H__

#include <vector>

class Coord
{
public:
	float x;
	float y;
	Coord(float, float);
	Coord();
};

class Vertex
{
public:
	Coord position;
	Coord direction;
	float speed;
	Vertex(Coord);
	Vertex();
};

class Lines // more general version of Vertex, we'll replace it later
{
public:
	std::vector<Coord> vertices;
	Coord direction;
	float speed;
	Lines(Coord);
	Lines();
};

#endif