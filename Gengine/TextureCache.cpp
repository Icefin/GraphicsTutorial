#include <iostream>

#include "TextureCache.h"
#include "ImageLoader.h"

namespace Gengine {
	TextureCache::TextureCache() {

	}
	TextureCache::~TextureCache() {

	}

	GLTexture TextureCache::GetTexture(std::string texturePath) {
		auto iter = _textureMap.find(texturePath);

		if (iter == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::LoadPNG(texturePath);
			
			_textureMap.insert(make_pair(texturePath, newTexture));

			return newTexture;
		}
		return iter->second;
	}
}