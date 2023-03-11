#include "Level.h"

#include <Gengine/Errors.h>
#include <Gengine/ResourceManager.h>

#include <fstream>
#include <iostream>

Level::Level(const std::string& fileName) {
	std::ifstream file;
	file.open(fileName);

	if (file.fail()) {
		Gengine::FatalError("Failed to open" + fileName);
	}
	
	std::string temp;
	file >> temp >> m_numHumans;

	std::getline(file, temp);
	while (std::getline(file, temp)) {
		m_levelData.push_back(temp);
	}

	m_spriteBatch.Init();
	m_spriteBatch.Begin();
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Gengine::ColorRGBA8 whiteColor(255, 255, 255, 255);
	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {
			char tile = m_levelData[y][x];
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			switch (tile) {
				case '.' :
					break;
				case 'B' :
				case 'R' :
					m_spriteBatch.Draw(destRect,
									uvRect,
									Gengine::ResourceManager::GetTexture("Textures/red_bricks.png").id,
									0.0f,
									whiteColor);
					break;
				case 'G' :
					m_spriteBatch.Draw(destRect,
									uvRect,
									Gengine::ResourceManager::GetTexture("Textures/glass.png").id,
									0.0f,
									whiteColor);
					break;
				case 'L':
					m_spriteBatch.Draw(destRect,
									uvRect,
									Gengine::ResourceManager::GetTexture("Textures/light_bricks.png").id,
									0.0f,
									whiteColor);
					break;
				case '@' :
					m_levelData[y][x] = '.';
					m_playerStartPos.x = x * TILE_WIDTH;
					m_playerStartPos.y = y * TILE_WIDTH;
					break;
				case 'Z' :
					m_levelData[y][x] = '.';
					m_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					break;
				default :
					std::printf("Unexpcted symbol %c at (%d, %d)\n", tile, x, y);
					break;
			}
		}
	}
	m_spriteBatch.End();
}

Level::~Level() {

}

void Level::draw() {
	m_spriteBatch.RenderBatchs();
}