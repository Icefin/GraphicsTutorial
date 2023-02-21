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

private :
	Gengine::SpriteBatch _spriteBatch;

	std::vector<std::string> _levelData;
	int _numHumans;

	glm::ivec2 _startPlayerPos;
	std::vector<glm::ivec2> _zombieStartPos;
};

