#include "Vector2.h"
#include <cmath>

Vector2::Vector2(double x, double y)
{
	this->x =x;
	this->y =y;
}

Vector2::Vector2(std::vector<double> v)
{
	x = v[0];
	y = v[1];
}
string Vector2::ToString() const
{
    stringstream sstream;
	sstream << "(" << x << "," << y << ")";
	return sstream.str();
}

Vector2 Vector2::Cross(Vector2 v1, Vector2 v2)
{
	Vector2 v;
	// v.x = (v1.y * v2.z) - (v1.z * v2.y);
	// v.y = (v1.x * v2.z) - (v1.z * v2.x);
	// v.z = (v1.x * v2.y) - (v1.y * v2.x);

	return v;
}
double Vector2::Dot(Vector2 v1, Vector2 v2)
{
	//a · b = ax × bx + ay × by + az * bz
	return (v1.x*v2.x) + (v1.y*v2.y);
}

void Vector2::Normalize()
{
	double magnitude = Vector2::Magnitude(*this);
	if( magnitude == 1.0)
		return; //Vector is already Normalized.
	this->x /= magnitude;
	this->y /= magnitude;
}

double Vector2::Magnitude(Vector2 v)
{
	return 	sqrt(v.x * v.x + v.y * v.y);
}
double Vector2::Magnitude(Vector2 v1, Vector2 v2)
{
	return Magnitude(v1-v2);
}

bool Vector2::IsAlongNormal(Vector2 normal, Vector2 point)
{
	double val=(Vector2::Dot(normal, point));
	// >0 = Along
	// <0 = Against
	//  0 = Perpendicular
	if(val>0)		
		return true;
	else
		return false; 
}