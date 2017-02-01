#include "Vertex.h"
#include <cmath>
	Vertex::Vertex(double x, double y, double z)
	{
		this->x =x;
		this->y =y;
		this->z =z;
	}

	string Vertex::ToString()
	{
	    stringstream sstream;
		sstream << "("<< x<<","<<y<<","<<z<<")";
		return sstream.str();
	}

Vertex Vertex::Cross(Vertex v1, Vertex v2)
{
	Vertex v;
	v.x = (v1.y * v2.z) - (v1.z * v2.y);
	v.y = (v1.x * v2.z) - (v1.z * v2.x);
	v.z = (v1.x * v2.y) - (v1.y * v2.x);

	return v;
}
double Vertex::Dot(Vertex v1, Vertex v2)
{
	//a · b = ax × bx + ay × by + az * bz
	return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
}

	void Vertex::Normalize()
	{
		double magnitude = Vertex::Magnitude(*this);
		if( magnitude == 1.0)
			return; //Vector is already Normalized.
		this->x /= magnitude;
		this->y /= magnitude;
		this->z /= magnitude;
	}

	double Vertex::Magnitude(Vertex v)
	{
		return 	sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
//Static Functions
	Vertex Vertex::Up(){return Vertex(0,1,0);}
	Vertex Vertex::Down(){return Vertex(0,-1,0);}
	Vertex Vertex::Left(){return Vertex(-1,0,0);}
	Vertex Vertex::Right(){return Vertex(1,0,0);}
	Vertex Vertex::Forward(){return Vertex(0,0,1);}
	Vertex Vertex::Backward(){return Vertex(0,0,-1);}