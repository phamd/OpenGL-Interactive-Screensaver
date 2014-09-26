#include "containers.h"
#include <vector>

Vector2d::Vector2d(float h, float v)
{
	x = h;
	y = v;
}

Vector2d::Vector2d() {}

Vector3d::Vector3d(float r, float g, float b)
{
	x = r;
	y = g;
	z = b;
}

Vector3d::Vector3d() {}

Vertex::Vertex(Vector2d pos, Vector2d dir) // insert first element
{
	position = pos;
	direction = dir;
}

Vertex::Vertex() {}

Drawable::Drawable(Vertex pos) // insert first element
{
	vertices.push_back(pos);
}

Drawable::Drawable(std::vector<Vertex> verti) // insert first element
{
	vertices = verti;
}

Drawable::Drawable() {}