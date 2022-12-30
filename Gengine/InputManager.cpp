#include "InputManager.h"


namespace Gengine {
	InputManager::InputManager() {
	
	}

	InputManager::~InputManager() {

	}

	void InputManager::PressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}
	void InputManager::ReleaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}
	bool InputManager::isKeyPressed(unsigned int keyID) {
		auto iter = _keyMap.find(keyID);
		if (iter != _keyMap.end()) {
			return iter->second;
		}

		return false;
	}
}