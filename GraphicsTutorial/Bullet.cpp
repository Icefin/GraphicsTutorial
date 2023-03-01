#include "Bullet.h"
#include <ResourceManager.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime) {
	_lifeTime = lifeTime;
	_position = pos;
	_direction = dir;
	_speed = speed;
}
Bullet::~Bullet() {

}

void Bullet::Draw(Gengine::SpriteBatch& spriteBatch) {
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Gengine::GLTexture texture = Gengine::ResourceManager::GetTexture("Textures/jimmyJump_pack/PNG/Bullet.png");
	
	Gengine::ColorRGBA8 color;
	color.r = 255; color.g = 255; color.b = 255; color.a = 255;

	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 5, 5);

	spriteBatch.Draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::Update() {
	_position += _direction * _speed;
	_lifeTime--;
	
	if (_lifeTime == 0) {
		return true;
	}
	return false;
}