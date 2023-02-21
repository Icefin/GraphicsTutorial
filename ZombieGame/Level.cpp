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
	file >> temp >> _numHumans;

	while (std::getline(file, temp)) {
		_levelData.push_back(temp);
	}

	_spriteBatch.Init();
	_spriteBatch.Begin();
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Gengine::Color whiteColor;
	whiteColor.r = 255; whiteColor.g = 255; whiteColor.b = 255; whiteColor.a = 255;
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			char tile = _levelData[y][x];
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			switch (tile) {
				case '.' :
					break;
				case 'B' :
				case 'R' :
					_spriteBatch.Draw(destRect,
									uvRect,
									Gengine::ResourceManager::GetTexture("Textures/red_bricks.png").id,
									0.0f,
									whiteColor);
					break;
				case 'G' :
					_spriteBatch.Draw(destRect,
						uvRect,
						Gengine::ResourceManager::GetTexture("Textures/glass.png").id,
						0.0f,
						whiteColor);
					break;
				case 'L':
					_spriteBatch.Draw(destRect,
						uvRect,
						Gengine::ResourceManager::GetTexture("Textures/light_bricks.png").id,
						0.0f,
						whiteColor);
					break;
				case '@' :
					_playerStartPos.x = x * TILE_WIDTH;
					_playerStartPos.y = y * TILE_WIDTH;
					break;
				case 'Z' :
					_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					break;
				default :
					std::printf("Unexpcted symbol %c at (%d, %d)\n", tile, x, y);
					break;
			}
		}
	}
	_spriteBatch.End();
}

Level::~Level() {

}

void Level::draw() {
	_spriteBatch.RenderBatchs();
}