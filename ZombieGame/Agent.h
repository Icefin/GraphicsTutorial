#pragma once
#include <glm/glm.hpp>
#include <Gengine/SpriteBatch.h>
#include <string>

const float AGENT_WIDTH = 60;

class Zombie;
class Human;

class Agent
{
public :
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*> humans,
						std::vector<Zombie*> zombies) = 0;

	void collideWithLevel(const std::vector<std::string>& levelData);

	void draw(Gengine::SpriteBatch& _spriteBatch);

	glm::vec2 getPosition() const { return _position; }

protected :
	void checkTilePosition(const std::vector<std::string>& levelData,
						std::vector<glm::vec2>& collideTilePositions,
						float x, float y);
	void collideWithTile(glm::vec2 tilePosition);

	glm::vec2 _position;
	Gengine::Color _color;
	float _speed;
};

