#include "Player.h"
#include <Gengine/ResourceManager.h>
#include <SDL/SDL.h>

Player::Player() {

}

Player::~Player() {

}

void Player::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, Gengine::ColorRGBA8 color) {
	Gengine::GLTexture texture = Gengine::ResourceManager::getTexture("Assets/blue_ninja.png");
	m_collisionBox.init(world, position, dimensions, texture, color, true, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f));
}

void Player::draw(Gengine::SpriteBatch& spriteBatch) {
	m_collisionBox.draw(spriteBatch);
}

void Player::update(Gengine::InputManager& inputManager) {
	b2Body* body = m_collisionBox.getBody();
	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	else {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y));
	}

	const float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}

	//Loop through all the contact points
	for (b2ContactEdge* edge = body->GetContactList(); edge != nullptr; edge = edge->next) {
		b2Contact* contactPoint = edge->contact;
		if (contactPoint->IsTouching()) {
			b2WorldManifold manifold;
			contactPoint->GetWorldManifold(&manifold);
			//Check if the points are below
			bool is_below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - m_collisionBox.getDimensions().y / 2.0f + 0.01f) {
					is_below = true;
					break;
				}
			}
			if (is_below) {
				//Jumpable Condition
				if (inputManager.isKeyPressed(SDLK_w)) {
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}