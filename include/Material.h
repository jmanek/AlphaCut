#pragma once

#include "Color.h"
#include <string>
	class Material
	{
		public:
			Material();
			std::string name;
			Color ambient;
			Color diffuse;
			std::string ambientMap;
			std::string diffuseMap;
			std::string alphaMap;
			// The basepath of the mtl file, assumes the textures are relative to this
			std::string basePath;
			std::string ToString();

			bool operator==(Material& t)
			{
				return (name == t.name && ambient == t.ambient && ambientMap == t.ambientMap && diffuse == t.diffuse && diffuseMap == t.diffuseMap);
			}

			bool const HasAmbientMap();
			bool const HasDiffuseMap();
			bool const HasAlphaMap();
	};
