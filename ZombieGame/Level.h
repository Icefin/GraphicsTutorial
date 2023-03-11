#pragma once
#include <Gengine/SpriteBatch.h>

#include <string>
#include <vector>

const int TILE_WIDTH = 64;

class Level
{
public :
	Level(const std::string& fileName);
	~Level();

	void draw();

	const std::vector<std::string>& getLevelData() const { return m_levelData; }
	glm::vec2 getPlayerStartPos() const { return m_playerStartPos; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return m_zombieStartPos; }
	int getNumHumans() const { return m_numHumans; }
	int getWidth() const { return m_levelData[0].size(); }
	int getHeight() const { return m_levelData.size(); }

private :
	Gengine::SpriteBatch m_spriteBatch;

	std::vector<std::string> m_levelData;
	int m_numHumans;

	glm::vec2 m_playerStartPos;
	std::vector<glm::vec2> m_zombieStartPos;
};

