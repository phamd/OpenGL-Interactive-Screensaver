#include "containers.h"
#include <vector>

Vector2d::Vector2d(float h, float v)
{
	x = h;
	y = v;
}

Vector2d::Vector2d()
{
	x = y = 1;
}

Vector3d::Vector3d(float r, float g, float b)
{
	x = r;
	y = g;
	z = b;
}

Vector3d::Vector3d()
{
	x = y = z = 1;
}

Vertex::Vertex(Vector2d pos, Vector2d dir)
{
	position = pos;
	direction = dir;
}

Vertex::Vertex()
{
	position = Vector2d(0, 0);
	direction = Vector2d(0, 0);
}

Shapes::Shapes(Vertex pos)
{
	vertices.push_back(pos);
}

Shapes::Shapes(std::vector<Vertex> verti)
{
	vertices = verti;
}

Shapes::Shapes() {}