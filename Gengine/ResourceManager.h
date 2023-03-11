#pragma once
#include "TextureCache.h"
#include <string>

namespace Gengine {
	class ResourceManager
	{
	public :
		static GLTexture getTexture(std::string texturePath);
	private :
		static TextureCache m_textureCache;
	};
}
