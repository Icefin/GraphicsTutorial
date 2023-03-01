#include "Human.h"

#include <glm/gtx/rotate_vector.hpp>

#include <ctime>
#include <random>

Human::Human() : _frames(0) {

}

Human::~Human() {

}

void Human::init(float speed, glm::vec2 pos) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_health = 20.0f;

	_color = Gengine::ColorRGBA8(200, 0, 200, 255);

	_speed = speed;
	_position = pos;
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	//make sure direction isn't zero
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*> humans,
	std::vector<Zombie*> zombies) {

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRot(-40.0f, 40.0f);

	_position += _direction * _speed;

	if (_frames == 1000) {
		_direction = glm::rotate(_direction, randRot(randomEngine));
		_frames = 0;
	}
	
	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRot(randomEngine));
	}
	_frames++;
}