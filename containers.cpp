#include "containers.h"
#include <vector>

Coord::Coord(float h, float v)
{
	x = h;
	y = v;
}

Coord::Coord()
{

}

Vertex::Vertex(Coord pos, Coord dir) // insert first element
{
	position = pos;
	direction = dir;
}

Vertex::Vertex()
{

}

Drawable::Drawable(Vertex pos) // insert first element
{
	vertices.push_back(pos);
}

Drawable::Drawable(std::vector<Vertex> verti) // insert first element
{
	vertices = verti;
}

Drawable::Drawable()
{

}