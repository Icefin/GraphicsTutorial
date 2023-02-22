#include "Agent.h"
#include "Level.h"
#include <Gengine/ResourceManager.h>
#include <algorithm>

Agent::Agent() {

}

Agent::~Agent() {

}

void Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;

	//Check the four corners around player
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);
	checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y);
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH);
	checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);
	
	if (collideTilePositions.size() == 0)
		return ;

	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
}

void Agent::draw(Gengine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static int textureID = Gengine::ResourceManager::GetTexture("Textures/circle.png").id;
	
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;
	
	spriteBatch.Draw(destRect, uvRect, textureID, 0.0f, _color);
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,
							std::vector<glm::vec2>& collideTilePositions,
							float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
									floor(y / (float)TILE_WIDTH));

	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y < 0 || cornerPos.y >= levelData.size())
		return;
	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}

void Agent::collideWithTile(glm::vec2 tilePosition) {
	const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPosition = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPosition - tilePosition;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 || yDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			if (distVec.x < 0)
				_position.x -= xDepth;
			else
				_position.x += xDepth;
		}
		else {
			if (distVec.y < 0)
				_position.y -= yDepth;
			else
				_position.y += yDepth;
		}
	}
}