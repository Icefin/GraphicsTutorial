#include "Agent.h"
#include <Gengine/ResourceManager.h>

Agent::Agent() {

}

Agent::~Agent() {

}

void Agent::draw(Gengine::SpriteBatch& _spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static int textureID = Gengine::ResourceManager::GetTexture("Textures/circle.png").id;
	
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;
	
	_spriteBatch.Draw(destRect, uvRect, textureID, 0.0f, _color);
}