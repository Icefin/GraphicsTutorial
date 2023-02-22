#pragma once
#include <string>
#include <vector>

#include <Gengine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public :
	Level(const std::string& fileName);
	~Level();

	void draw();

	const std::vector<std::string>& getLevelData() const { return _levelData; }
	glm::vec2 getPlayerStartPos() const { return _playerStartPos; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return _zombieStartPos; }
	int getNumHumans() const { return _numHumans; }
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }

private :
	Gengine::SpriteBatch _spriteBatch;

	std::vector<std::string> _levelData;
	int _numHumans;

	glm::vec2 _playerStartPos;
	std::vector<glm::vec2> _zombieStartPos;
};

