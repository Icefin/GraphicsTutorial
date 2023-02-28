#include "Gun.h"

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed) :
	_name(name),
	_fireRate(fireRate),
	_bulletsPerShot(bulletsPerShot),
	_spread(spread),
	_bulletDamage(bulletDamage),
	_bulletSpeed(bulletSpeed),
	_frameCounter(0) {

}

Gun::~Gun() {

}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets) {
	_frameCounter++;
	if (_frameCounter >= _fireRate && isMouseDown) {
		fire(direction, position, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets) {
	for (int i = 0; i < _bulletsPerShot; i++) {
		bullets.emplace_back(position, direction, _bulletDamage, _bulletSpeed);
	}
}