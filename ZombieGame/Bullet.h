#pragma once

#include <Gengine/SpriteBatch.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Agent;
class Human;
class Zombie;

const int BULLET_RADIUS = 5;

class Bullet
{
public :
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	bool update(const std::vector<std::string>& levelData, float deltaTime);
	void draw(Gengine::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* other);

	float getDamage() const { return (_damage); }
	glm::vec2 getPosition() const { return (_position); }

private :
	bool collideWithWorld(const std::vector<std::string>& levelData);

	glm::vec2 _position;
	glm::vec2 _direction;
	float _damage;
	float _speed;
};

