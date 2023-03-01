#include "InputManager.h"


namespace Gengine {
	InputManager::InputManager() : _mouseCoords(0.0f, 0.0f) {
	
	}

	InputManager::~InputManager() {

	}

	void InputManager::update() {
		for (auto& key : _keyMap) {
			_prevKeyMap[key.first] = key.second;
		}
	}

	void InputManager::PressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}
	void InputManager::ReleaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	void InputManager::SetMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID) {
		auto iter = _keyMap.find(keyID);
		if (iter != _keyMap.end()) {
			return iter->second;
		}
		return false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		if (isKeyDown(keyID) && wasKeyDown(keyID) == false) {
			return (true);
		}
		return (false);
	}

	bool InputManager::wasKeyDown(unsigned int keyID) {
		auto iter = _prevKeyMap.find(keyID);
		if (iter != _prevKeyMap.end()) {
			return iter->second;
		}

		return false;
	}
}