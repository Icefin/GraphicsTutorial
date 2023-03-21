#pragma once

#include "Box.h"
#include <Gengine/SpriteBatch.h>
#include <Gengine/GLTexture.h>
#include <Gengine/InputManager.h>

class Player
{
public :
	Player();
	~Player();

	void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, Gengine::ColorRGBA8 color);
	void draw(Gengine::SpriteBatch& spriteBatch);
	void update(Gengine::InputManager& inputManager);

	const Box& getBox() const { return (m_collisionBox); }

private :
	Box m_collisionBox;
};

