#pragma once

#include "../Image.h"


Image invert(const Image& src);
Image grayscale(const Image& src);
Image gaussianBlur(const Image& src, int radius);