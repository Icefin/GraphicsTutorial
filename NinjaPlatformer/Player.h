#pragma once

#include "Capsule.h"
#include <Gengine/SpriteBatch.h>
#include <Gengine/GLTexture.h>
#include <Gengine/InputManager.h>
#include <Gengine/DebugRenderer.h>
#include <Gengine/TileSheet.h>

enum class PlayerState {
	STANDING,
	RUNNING,
	PUNCHING,
	JUMPING
};

class Player
{
public :
	void init(b2World* world,
		const glm::vec2& position,
		const glm::vec2& drawDims,
		const glm::vec2& collisionsDims,
		Gengine::ColorRGBA8 color);
	void draw(Gengine::SpriteBatch& spriteBatch);
	void drawDebug(Gengine::DebugRenderer& debugRenderer);
	void update(Gengine::InputManager& inputManager);

	const Capsule& getCapsule() const { return (m_capsule); }
	const glm::vec2 getPosition() const {
		glm::vec2 res;
		res.x = m_capsule.getBody()->GetPosition().x;
		res.y = m_capsule.getBody()->GetPosition().y;
		return (res);
	}

private :
	glm::vec2 m_drawDims;
	Gengine::TileSheet m_texture;
	Gengine::ColorRGBA8 m_color;
	Capsule m_capsule;

	PlayerState m_state = PlayerState::STANDING;
	bool m_onGround = false;
	bool m_isPunching = false;
	int	m_direction = 1;
	float m_animTime = 0.0f;
};

