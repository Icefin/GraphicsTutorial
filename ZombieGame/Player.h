#pragma once
#include "Human.h"

#include <Gengine/InputManager.h>

class Player : public Human
{
public :
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Gengine::InputManager* inputManager);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*> humans,
		std::vector<Zombie*> zombies);

private :
	Gengine::InputManager* _inputManager;
};

