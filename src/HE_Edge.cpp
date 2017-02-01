#include "HE_Edge.h"
#include "Vertex.h"
#include <vector>


HE_Edge::HE_Edge(HE_Face* face, HE_Vertex *vA, HE_Vertex* vB) {
	this->face = face;
	this->vA = vA;
	this->vB = vB;
}

HE_Edge::HE_Edge(HE_Face* face, HE_Vertex *vA, HE_Vertex* vB, TextureCoord* vtA, TextureCoord* vtB) {
	this->face = face;
	this->vA = vA;
	this->vB = vB;
	this->vtA = vtA;
	this->vtB = vtB;
}


string HE_Edge::ToString()
{
    stringstream sstream;
	sstream << "("<< vA->ToString() <<","<<vB->ToString()<<")";
	return sstream.str();
}

bool HE_Edge::IsSingular()
{
	return oppositeEdges.size() > 1;
}
bool HE_Edge::IsBoundary()
{
	return oppositeEdges.size() == 0;
}
bool HE_Edge::OrientationIsConsistent(HE_Edge* e)
{
	return *vA == *(e->vB) && *vB == *(e->vA);
}

std::vector<double> HE_Edge::GetMidpoint()
{
    std::vector<double> mp;
    mp.push_back((vA->x + vB->x)/2);
    mp.push_back((vB->y + vB->y)/2);
    mp.push_back((vA->z + vB->z)/2);
    return mp;
}
