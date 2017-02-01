#include "Image.h"
#define IMAGE_DEPTH 0
using namespace cimg_library;
using namespace std;
Image::Image(const std::string &fp)
{
	cimg = CImg<unsigned char>(fp.c_str());
}

int Image::Width() const
{
	return cimg.width();
}

int Image::Height() const
{
	return cimg.height();
}

int Image::Spectrum() const
{
	return cimg.spectrum();
}

bool Image::HasAlpha() const
{
	return Spectrum() == 4;
}

Color Image::Pixel(int x, int y) const
{
	Color pixel ((float)cimg(x, y, IMAGE_DEPTH, 0)/255.0f,
				 (float)cimg(x, y, IMAGE_DEPTH, 1)/255.0f,
				 (float)cimg(x, y, IMAGE_DEPTH, 2)/255.0f);
	if (HasAlpha()) { pixel.a = (float)cimg(x, y, IMAGE_DEPTH, 3)/255.0f; }
	return pixel;
}

void Image::PrintPixel(int x, int y) const
{
	cout << "R: " << (int) cimg(x, y, IMAGE_DEPTH, 0) <<
		    " G: " << (int) cimg(x, y, IMAGE_DEPTH, 1) <<
		    " B: " << (int) cimg(x, y, IMAGE_DEPTH, 2);
	if (HasAlpha()) { cout << " A: " << (int) cimg(x, y, IMAGE_DEPTH, 3); }
	cout << endl;
}

bool Image::IsTransparent(int x, int y) const
{
	return cimg(x, y, IMAGE_DEPTH, 3) == 0;
}
