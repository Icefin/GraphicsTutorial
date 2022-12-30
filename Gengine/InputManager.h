#pragma once
#include <unordered_map>

#include <glm/glm.hpp>

namespace Gengine {
	class InputManager
	{
	public :
		InputManager();
		~InputManager();

		void PressKey(unsigned int keyID);
		void ReleaseKey(unsigned int keyID);

		void SetMouseCoords(float x, float y);

		bool isKeyPressed(unsigned int keyID);

		glm::vec2 GetMouseCoords() const { return _mouseCoords; }

	private :
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
	};
}
