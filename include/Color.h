#pragma once

#include <string>

	class Color
	{
		public:
			Color();
			Color(float r, float g, float b);
			Color(float r, float g, float b, float a);
			Color(unsigned char r, unsigned char g, unsigned char b);

			float r;
			float g;
			float b;
			float a;

			std::string ToString();
			bool operator==(const Color& t)
			{
				return (r == t.r && g == t.g && b == t.b);
			}
			void Set(float r, float g, float b);
			void Set(float r, float g, float b, float a);
	};
