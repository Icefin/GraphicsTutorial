#pragma once
#include "Bullet.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <Gengine/AudioEngine.h>

class Gun
{
public :
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, Gengine::SoundEffect fireEffect);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);

private :
	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);

	std::string _name;
	Gengine::SoundEffect _fireEffect;

	int _fireRate;
	int _bulletsPerShot;
	float _spread;

	float _bulletDamage;
	float _bulletSpeed;

	float _frameCounter;
};

