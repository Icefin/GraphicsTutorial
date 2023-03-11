#include <iostream>

#include "TextureCache.h"
#include "ImageLoader.h"

namespace Gengine {
	TextureCache::TextureCache() {

	}
	TextureCache::~TextureCache() {

	}

	GLTexture TextureCache::getTexture(std::string texturePath) {
		auto iter = m_textureMap.find(texturePath);

		if (iter == m_textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
			
			m_textureMap.insert(make_pair(texturePath, newTexture));

			return (newTexture);
		}
		return (iter->second);
	}
}