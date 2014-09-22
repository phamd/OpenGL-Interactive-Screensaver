
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