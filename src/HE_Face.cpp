#include "HE_Face.h"

HE_Face::HE_Face()
{
	materialIndex = -1;
}

HE_Face::HE_Face(long unsigned int  index, HE_Vertex* vA, HE_Vertex* vB, HE_Vertex* vC)
{
	this->uniqueID = index;
//	this->vA = vA;
//	this->vB = vB;
//	this->vC = vC;
	materialIndex = -1;
}

HE_Vertex HE_Face::Normal()
{
	HE_Vertex u = *(edge->vB) - *(edge->vA);
	HE_Vertex v = *(edge->prevEdge->vA) - *(edge->vA);	
	HE_Vertex n = HE_Vertex::Cross(u, v);
	n.Normalize();
	return n;
}

void HE_Face::Flip()
{
	cout << "e1" << endl;
	HE_Edge* e1;
	cout << "e1" << endl;
	*e1 = *edge;
	HE_Edge* e2;
	cout << "e2" << endl;
	*e2 = *(edge->nextEdge);
	HE_Edge* e3;
	*e3 = *(edge->prevEdge);
	// e1
	HE_Vertex* nvA = e1->vA;
	HE_Vertex* nvB = e1->vB;
	e1->vA = nvB;
	e1->vB = nvA;
	e1->nextEdge = e3;
	e1->prevEdge = e2;
	// e3 -> e2
	nvA = e3->vA;
	nvB = e3->vB;
	e3->vA = nvB;
	e3->vB = nvA;
	e3->nextEdge = e2;
	e3->prevEdge = e1;
	// e2 -> e3
	nvA = e2->vA;
	nvB = e2->vB;
	e2->vA = nvB;
	e2->vB = nvA;
	e2->nextEdge = e1;
	e2->prevEdge = e3;
}

HE_Vertex* HE_Face::vA()
{
	return edge->vA;
}
HE_Vertex* HE_Face::vB()
{
	return edge->nextEdge->vA;
}
HE_Vertex* HE_Face::vC()
{
	return edge->prevEdge->vA;
}
TextureCoord* HE_Face::vtA()
{
	return edge->vtA;
}
TextureCoord* HE_Face::vtB()
{
	return edge->nextEdge->vtA;
}
TextureCoord* HE_Face::vtC()
{
	return edge->prevEdge->vtA;
}
bool HE_Face::IsBoundary()
{
    return edge->IsBoundary() || edge->nextEdge->IsBoundary() || edge->prevEdge->IsBoundary();
}
    

string HE_Face::ToString()
{
    stringstream sstream;
	sstream << uniqueID <<":("<< vA()->ToString() <<","<<vB()->ToString()<< vC()->ToString()<<")";
	return sstream.str();
}
