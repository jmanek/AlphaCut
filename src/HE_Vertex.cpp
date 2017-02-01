#include "HE_Vertex.h"

long unsigned int HE_Vertex::lastID =0;

HE_Vertex::HE_Vertex(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


void HE_Vertex::AssignFace( HE_Face* face)
{
	this->faces.push_back(face);
}
// void HE_Vertex::AssignEdge( shared_ptr<HE_Edge> edge)
// {
// 	this->edgeList.push_back(edge);
// }
