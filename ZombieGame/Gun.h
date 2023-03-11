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

	std::string m_name;
	Gengine::SoundEffect m_fireEffect;

	int m_fireRate;
	int m_bulletsPerShot;
	float m_spread;

	float m_bulletDamage;
	float m_bulletSpeed;

	float m_frameCounter;
};

