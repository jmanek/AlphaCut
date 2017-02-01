#include "Material.h"
#include <string>
#include <sstream>

using namespace std;

Material::Material() 
{
	name = "";
	ambient = Color();
	ambientMap = "";
	diffuse = Color();
	diffuseMap = "";
	alphaMap = "";
	basePath = "";
}


string Material::ToString()
{
    stringstream sstream;
    sstream.str("");
	sstream << name << ambient.ToString() << ambientMap << diffuse.ToString() << diffuseMap << alphaMap << basePath;
	return sstream.str();
}

bool const Material::HasAmbientMap()
{
	return ambientMap != "";
}

bool const Material::HasDiffuseMap()
{
	return diffuseMap != "";
}

bool const Material::HasAlphaMap()
{
	return alphaMap != "";
}
