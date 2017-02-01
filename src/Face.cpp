#include "Face.h"


Face::Face()
{}

Face::Face(unsigned int vA, unsigned int vB, unsigned int vC, unsigned int vtA, unsigned int vtB, unsigned int vtC, unsigned int materialIndex)
{
	this->vA = vA;
	this->vB = vB;
	this->vC = vC;
	this->vtA = vtA;
	this->vtB = vtB;
	this->vtC = vtC;
	this->materialIndex = materialIndex;
}

Face::Face(unsigned int vA, unsigned int vB, unsigned int vC)
{
	this->vA = vA;
	this->vB = vB;
	this->vC = vC;
}
