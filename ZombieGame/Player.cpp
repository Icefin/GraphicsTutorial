#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::init(float speed, glm::vec2 pos) {
	_speed = speed;
	_position = pos;
	_color.r = 0; _color.g = 0; _color.b = 128; _color.a = 255;
}

void Player::update() {

}