#pragma once
#include "Human.h"
#include "Bullet.h"

#include <Gengine/InputManager.h>
#include <Gengine/Camera2D.h>

class Gun;

class Player : public Human
{
public :
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, Gengine::InputManager* inputManager, Gengine::Camera2D* camera, std::vector<Bullet>* bullets);
	void update(const std::vector<std::string>& levelData,
		std::vector<Human*> humans,
		std::vector<Zombie*> zombies,
		float deltaTime) override;
	void addGun(Gun* gun);

private :
	Gengine::InputManager* _inputManager;
	Gengine::Camera2D* _camera;

	std::vector<Gun*> _guns;
	int _currentGunIndex;
	std::vector<Bullet>* _bullets;
};

