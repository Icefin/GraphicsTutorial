#pragma once
#include "Human.h"
#include <Gengine/InputManager.h>

class Player : public Human
{
public :
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Gengine::InputManager* inputManager);

	void update();

private :
	Gengine::InputManager* _inputManager;
};

