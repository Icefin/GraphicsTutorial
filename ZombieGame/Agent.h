#pragma once
#include <glm/glm.hpp>
#include <Gengine/SpriteBatch.h>

const float AGENT_WIDTH = 60;

class Agent
{
public :
	Agent();
	virtual ~Agent();

	void draw(Gengine::SpriteBatch& _spriteBatch);

protected :
	glm::vec2 _position;
	Gengine::Color _color;
	float _speed;
};

