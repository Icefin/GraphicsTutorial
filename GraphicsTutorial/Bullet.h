#pragma once

#include <glm/glm.hpp>
#include <SpriteBatch.h>

class Bullet
{
public :
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void Draw(Gengine::SpriteBatch& spriteBatch);
	bool Update();

private :
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

