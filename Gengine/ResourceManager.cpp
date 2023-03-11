#include "ResourceManager.h"

namespace Gengine {
	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return (m_textureCache.getTexture(texturePath));
	}

	TextureCache ResourceManager::m_textureCache;
}