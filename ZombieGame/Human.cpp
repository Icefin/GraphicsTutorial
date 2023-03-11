#include "Human.h"

#include <glm/gtx/rotate_vector.hpp>
#include <Gengine/ResourceManager.h>

#include <ctime>
#include <random>

Human::Human() : m_frames(0) {

}

Human::~Human() {

}

void Human::init(float speed, glm::vec2 pos) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_health = 20.0f;

	m_color = Gengine::ColorRGBA8(255, 255, 255, 255);

	m_speed = speed;
	m_position = pos;
	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	//make sure direction isn't zero
	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

	m_direction = glm::normalize(m_direction);
	m_textureID = Gengine::ResourceManager::GetTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*> humans,
	std::vector<Zombie*> zombies,
	float deltaTime) {

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRot(-40.0f, 40.0f);

	m_position += m_direction * m_speed * deltaTime;

	if (m_frames == 1000) {
		m_direction = glm::rotate(m_direction, randRot(randomEngine));
		m_frames = 0;
	}
	
	if (collideWithLevel(levelData)) {
		m_direction = glm::rotate(m_direction, randRot(randomEngine));
	}
	m_frames++;
}