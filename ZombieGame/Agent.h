#pragma once
#include <glm/glm.hpp>
#include <Gengine/SpriteBatch.h>

const float AGENT_WIDTH = 60;

class Agent
{
public :
	Agent();
	virtual ~Agent();

	virtual void update() = 0;

	void draw(Gengine::SpriteBatch& _spriteBatch);

	glm::vec2 getPosition() const { return _position; }

protected :
	glm::vec2 _position;
	Gengine::Color _color;
	float _speed;
};

