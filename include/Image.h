#pragma once
#define cimg_use_png
#include "CImg.h"
#include "Color.h"
#include <string>
#include <iostream>
#include <vector>
	using namespace cimg_library;
	class Image
	{
		public:
			Image(const std::string &fp);
			CImg<unsigned char> cimg;
			// The number of pixels in the x-dimension
			int Width() const;
			// The number of pixels in the y-dimension
			int Height() const;
			// The number of channels per pixel
			int Spectrum() const;
			//Returns true if the image has an alpha channel
			bool HasAlpha() const;
			// Returns the Color at the pixel
			Color Pixel(int x, int y) const;
			// Returns true if the image is transparent at the pixel
			bool IsTransparent(int x, int y) const;
			void PrintPixel(int x, int y) const;

	};
