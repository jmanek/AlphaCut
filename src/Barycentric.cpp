#include "Barycentric.h"
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
using namespace std;

Barycentric::Barycentric(double u, double v, double w)
{
	this->u = u;
	this->v = v;
	this->w = w;
}


string Barycentric::ToString()
{
    stringstream sstream;
	sstream << "(" << u << ", " << v << ", " << w << ")";
	return sstream.str();
}

bool Barycentric::IsValid()
{
	if (u < 0.0 || v < 0.0 || w < 0.0) { return false; }
	if (abs( 1.0 - u - v - w) > 0.000001) { return false; }
	return true;
}