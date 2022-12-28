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
		
			std::cout << "Loaded Texture!\n";
			return newTexture;
		}

		std::cout << "Used Cached Texture!\n";
		return iter->second;
	}
}