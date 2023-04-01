#pragma once

#include <glm/glm.hpp>
#include <Gengine/Vertex.h>
#include <Gengine/SpriteBatch.h>

class Light {
public :
	void draw(Gengine::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;
		destRect.x = position.x - size / 2.0f;
		destRect.y = position.y - size / 2.0f;
		destRect.z = size;
		destRect.w = size;
		spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f) /*unit circle (0,0)*/, 0, 0.0f, color, 0.0f);
	}

	Gengine::ColorRGBA8 color;
	glm::vec2 position;
	float size;
};