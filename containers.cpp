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

Vertex::Vertex(Coord pos) // insert first element
{
	position = pos;
	direction = Coord(0, 0);
	speed = 0;
}

Vertex::Vertex()
{
}

Lines::Lines(Coord pos) // insert first element
{
	vertices.push_back(pos);
	direction = Coord(0, 0);
	speed = 0;
}

Lines::Lines()
{

}