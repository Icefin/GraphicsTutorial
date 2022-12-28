#pragma once
#include "TextureCache.h"
#include <string>

namespace Gengine {
	class ResourceManager
	{
	public :
		static GLTexture GetTexture(std::string texturePath);
	private :
		static TextureCache _textureCache;
	};
}
