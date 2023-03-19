#pragma once
#include <glm/glm.hpp>
#include <Gengine/Vertex.h>

struct Cell;

struct Ball {
	Ball(float radius, float mass, const glm::vec2& pos,
		const glm::vec2& vel, unsigned int textureID,
		const Gengine::ColorRGBA8& color);

	float radius;
	float mass;
	glm::vec2 position;
	glm::vec2 velocity;
	unsigned int textureID;
	Gengine::ColorRGBA8 color;
	Cell* ownerCell = nullptr;
	int cellVectorIdx = -1;
};

