#include "TextureCoord.h"
#include <string>
#include <sstream>     
using namespace std;


TextureCoord::TextureCoord(Vector2 v)
{
	x = v.x;
	y = v.y;
}
void TextureCoord::AssignFace( HE_Face* face)
{
	this->faces.push_back(face);
}

// string TextureCoord::ToString()
// {
//     stringstream sstream;
// 	sstream << "(" << x << "," << y << ")";
// 	return sstream.str();
// }
