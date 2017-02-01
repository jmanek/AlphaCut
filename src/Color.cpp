#include "Color.h"
#include <string>
#include <sstream>

using namespace std;

Color::Color() 
{
	r = 0.f;
	g = 0.f;
	b = 0.f;
	a = 1.0f;
}

Color::Color(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


string Color::ToString()
{
    stringstream sstream;
	sstream << "R: " << r << " G: " << g << " B: "  << b << " A: " << a;
	return sstream.str();
}

void Color::Set(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

void Color::Set(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}