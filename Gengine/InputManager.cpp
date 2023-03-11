#include "InputManager.h"


namespace Gengine {
	InputManager::InputManager() : m_mouseCoords(0.0f, 0.0f) {
	
	}

	InputManager::~InputManager() {

	}

	void InputManager::update() {
		for (auto& key : m_keyMap) {
			m_prevKeyMap[key.first] = key.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID) {
		m_keyMap[keyID] = true;
	}
	void InputManager::releaseKey(unsigned int keyID) {
		m_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y) {
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID) {
		auto iter = m_keyMap.find(keyID);
		if (iter != m_keyMap.end()) {
			return (iter->second);
		}
		return (false);
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		if (isKeyDown(keyID) && wasKeyDown(keyID) == false) {
			return (true);
		}
		return (false);
	}

	bool InputManager::wasKeyDown(unsigned int keyID) {
		auto iter = m_prevKeyMap.find(keyID);
		if (iter != m_prevKeyMap.end()) {
			return (iter->second);
		}

		return (false);
	}
}