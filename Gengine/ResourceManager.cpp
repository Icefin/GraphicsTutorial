#include "ResourceManager.h"

namespace Gengine {
	GLTexture ResourceManager::GetTexture(std::string texturePath) {
		return _textureCache.GetTexture(texturePath);
	}

	TextureCache ResourceManager::_textureCache;
}