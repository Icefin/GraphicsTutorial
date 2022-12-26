#pragma once
#include "GLTexture.h"
#include <string>

namespace Gengine {
	class ImageLoader
	{
	public:
		static GLTexture LoadPNG(std::string filePath);
	};
}