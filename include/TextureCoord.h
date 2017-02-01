#pragma once

#include "HE_Face.h"
#include <string>
#include "Vector2.h"
	class TextureCoord : public Vector2
	{
		public:
			TextureCoord() : Vector2() {};
			TextureCoord(double x, double y) : Vector2(x, y) {};
			TextureCoord(vector<double> v) : Vector2(v) {};
			TextureCoord(Vector2 v);
			void AssignFace(HE_Face* face);
			std::vector<HE_Face*> faces;
			// string ToString();
	};
