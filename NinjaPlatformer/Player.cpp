#include "Player.h"
#include <Gengine/ResourceManager.h>
#include <SDL/SDL.h>

void Player::init(b2World* world,
	const glm::vec2& position,
	const glm::vec2& drawDims,
	const glm::vec2& collisionsDims,
	Gengine::ColorRGBA8 color) {
	m_drawDims = drawDims;
	Gengine::GLTexture texture = Gengine::ResourceManager::getTexture("Assets/blue_ninja.png");
	m_color = color;
	m_capsule.init(world, position, collisionsDims, 1.0f, 0.1f, true);
	m_texture.init(texture, glm::ivec2(10, 2));
}

void Player::draw(Gengine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect;
	b2Body* body = m_capsule.getBody();
	destRect.x = body->GetPosition().x - m_drawDims.x / 2.0f;
	destRect.y = body->GetPosition().y - m_capsule.getDimensions().y / 2.0f;
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;

	int tileIdx;
	int numTiles;

	float animSpeed = 0.2f;
	glm::vec2 velocity;
	velocity.x = body->GetLinearVelocity().x;
	velocity.y = body->GetLinearVelocity().y;

	//Calculate Animation
	if (m_onGround) {
		if (m_isPunching) {
			numTiles = 4;
			tileIdx = 1;
			if (m_state != PlayerState::PUNCHING) {
				m_state = PlayerState::PUNCHING;
				m_animTime = 0.0f;
			}
		}
		else if (abs(velocity.x) > 1.0f && (velocity.x > 0 && m_direction > 0) || (velocity.x < 0 && m_direction < 0)) {
			numTiles = 6;
			tileIdx = 10;
			animSpeed = abs(velocity.x) * 0.025f;
			if (m_state != PlayerState::RUNNING) {
				m_state = PlayerState::RUNNING;
				m_animTime = 0.0f;
			}
		}
		else {
			numTiles = 1;
			tileIdx = 0;
			m_state = PlayerState::STANDING;
		}
	}
	else {
		if (m_isPunching) {
			numTiles = 1;
			tileIdx = 18;
			animSpeed *= 0.25f;
			if (m_state != PlayerState::PUNCHING) {
				m_state = PlayerState::PUNCHING;
				m_animTime = 0.0f;
			}
		}
		else if (abs(velocity.x) > 10.0f) {
			numTiles = 1;
			tileIdx = 10;
			m_state = PlayerState::JUMPING;
		}
		else if (velocity.y <= 0.0f) {
			numTiles = 1;
			tileIdx = 17;
			m_state = PlayerState::JUMPING;
		}
		else {
			numTiles = 1;
			tileIdx = 16;
			m_state = PlayerState::JUMPING;
		}
	}

	m_animTime += animSpeed;

	if (m_animTime > numTiles) {
		m_isPunching = false;
	}

	tileIdx = tileIdx + (int)m_animTime % numTiles;

	glm::vec4 uvRect = m_texture.getUVs(tileIdx);

	if (m_direction == -1) {
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}

	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, m_color, body->GetAngle());
}

void Player::drawDebug(Gengine::DebugRenderer& debugRenderer) {
	m_capsule.drawDebug(debugRenderer);
}

void Player::update(Gengine::InputManager& inputManager) {
	b2Body* body = m_capsule.getBody();
	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		m_direction = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
		m_direction = 1;
	}
	else {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y));
	}

	if (inputManager.isKeyPressed(SDLK_SPACE)) {
		m_isPunching = true;
	}

	const float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}

	m_onGround = false;
	//Loop through all the contact points
	for (b2ContactEdge* edge = body->GetContactList(); edge != nullptr; edge = edge->next) {
		b2Contact* contactPoint = edge->contact;
		if (contactPoint->IsTouching()) {
			b2WorldManifold manifold;
			contactPoint->GetWorldManifold(&manifold);
			//Check if the points are below
			bool is_below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + m_capsule.getDimensions().x / 2.0f + 0.01f) {
					is_below = true;
					break;
				}
			}
			if (is_below) {
				m_onGround = true;
				//Jumpable Condition
				if (inputManager.isKeyPressed(SDLK_w)) {
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}