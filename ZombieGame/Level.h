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

	glm::vec2 getPlayerStartPos() const { return _playerStartPos; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return _zombieStartPos; }

private :
	Gengine::SpriteBatch _spriteBatch;

	std::vector<std::string> _levelData;
	int _numHumans;

	glm::vec2 _playerStartPos;
	std::vector<glm::vec2> _zombieStartPos;
};

