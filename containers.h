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
	Vertex(Coord, Coord);
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
