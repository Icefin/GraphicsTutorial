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
	Gengine::InputManager* m_inputManager;
	Gengine::Camera2D* m_camera;

	std::vector<Gun*> m_guns;
	int m_currentGunIndex;
	std::vector<Bullet>* m_bullets;
};

